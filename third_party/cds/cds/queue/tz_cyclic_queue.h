/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_QUEUE_TZ_CYCLIC_QUEUE_H
#define    __CDS_QUEUE_TZ_CYCLIC_QUEUE_H

/*
    File: tz_cyclic_queue.h

    Source:
        [2000] Philippas Tsigas, Yi Zhang "A Simple, Fast and Scalable Non-Blocking Concurrent FIFO Queue
            for Shared Memory Multiprocessor Systems"

    Editions:
        2009.04.18  Maxim.Khiszinsky    Refactoring
        2007.06.17    Maxim.Khiszinsky    Created

*/

#include <cds/queue/details/queue_base.h>
#include <cds/details/markptr.h>
#include <cds/details/allocator.h>

//@cond
namespace cds {
    namespace queue {

        // forward declaration
        template < typename T,
            class TRAITS = traits,
            class ALLOCATOR = CDS_DEFAULT_ALLOCATOR >
        class TZCyclicQueue ;

        /// TZCyclicQueue specialization for pointer.
        template < typename T, class TRAITS, class ALLOCATOR >
        class TZCyclicQueue< T *, TRAITS, ALLOCATOR >: public concept::bounded_container
        {
            typedef    atomic32u_t        TIndex    ;

            const TIndex            m_nCapacity    ;    //< array capacity; 2**n - 1, also bit mask (instead of modulo division)

            atomic<TIndex>  m_nHead        ;    //< index of queue's head
            atomic<TIndex>  m_nTail        ;    //< index of queue's tail
            T **            m_Array        ;    //< array of pointer T *, array size is equal to m_nCapacity+1

            static T * free0()                { return NULL;        }
            static T * free1()                { return (T*) 1;    }
            static bool isFree( T * p )        { return p == free0() || p == free1(); }

        public:
            typedef T *        value_type    ;
            typedef normalized_type_traits<TRAITS>            type_traits ;

        protected:
            cds::details::Allocator< T *, ALLOCATOR >   m_Alloc ;       ///< Node allocator
            typename type_traits::item_counter_type        m_ItemCounter;    ///< Item counter

        public:
            /// Constructs queue
            /**
                The capacity of cyclic array is 2**nPow2, nPow2 <= 31
            */
            TZCyclicQueue( unsigned int nPow2 = 20 )
                : m_nCapacity( (1 << nPow2) - 1 ),  // used as bitmask (modulo division)
                m_nHead(0),
                m_nTail(1)
            {
                assert( nPow2 < CDS_BUILD_BITS ) ;
                m_Array = m_Alloc.NewArray( m_nCapacity + 1 ) ;
                memset( m_Array, 0, (m_nCapacity + 1) * sizeof( m_Array[0] ) ) ;
                m_Array[0] = free1()        ;
            }
            ~TZCyclicQueue()
            {
                m_Alloc.Delete( m_Array, capacity() ) ;
            }

            size_t    capacity() const            { return m_nCapacity + 1 ; }

            bool enqueue( T * pNewNode )
            {
                assert( !cds::details::isMarkedBit( pNewNode ) ) ; // младший бит используется как метка
                typename type_traits::backoff_strategy bkoff    ;

                const TIndex nModulo = m_nCapacity    ;

                do {
                    TIndex te = m_nTail.load<membar_acquire>()    ;
                    TIndex ate = te        ;
                    T * tt = m_Array[ ate ]    ;
                    TIndex temp = ( ate + 1 ) & nModulo ;    // next item after tail

                    // Looking for actual tail
                    while ( !isFree( tt ) ) {
                        if ( te != m_nTail.load<membar_acquire>() )    // check the tail consistency
                            goto TryAgain    ;
                        if ( temp == m_nHead.load<membar_acquire>() )    // queue full?
                            break ;
                        tt = m_Array[ temp ];
                        ate = temp            ;
                        temp = (ate + 1) & nModulo ;
                    }

                    if ( te != m_nTail.load<membar_acquire>() )
                        continue ;

                    // Check whether queue is full
                    if ( temp == m_nHead.load<membar_acquire>() ) {
                        ate = ( temp + 1 ) & nModulo ;
                        tt = m_Array[ ate ] ;
                        if ( !isFree( tt ) ) {
                            return false    ;    // Queue is full
                        }

                        // help the dequeue to update head
                        m_nHead.cas<membar_release>( temp, ate ) ;
                        continue ;
                    }

                    if ( tt == free1() )
                        pNewNode = cds::details::markBit( pNewNode ) ;
                    if ( te != m_nTail.load<membar_acquire>() )
                        continue    ;

                    // get actual tail and try to enqueue new node
                    if ( atomics::cas<membar_release>( m_Array + ate, tt, pNewNode ) ) {
                        if ( temp % 2 == 0 )
                            m_nTail.cas<membar_release>( te, temp ) ;
                        ++m_ItemCounter    ;
                        return true ;
                    }
                TryAgain:    ;
                } while ( bkoff(), true )    ;

                // No control path reaches this line!
                return false    ;
            }

            bool dequeue( T *& dest )
            {
                typename type_traits::backoff_strategy bkoff    ;

                const TIndex nModulo = m_nCapacity    ;
                do {
                    TIndex th = m_nHead.load<membar_acquire>()  ;
                    TIndex temp = ( th + 1 ) & nModulo ;
                    T * tt = m_Array[ temp ] ;
                    T * pNull    ;

                    // find the actual head after this loop
                    while ( isFree( tt ) ) {
                        if ( th != m_nHead.load<membar_acquire>() )
                            goto TryAgain ;
                        // two consecutive NULL means queue empty
                        if ( temp == m_nTail.load<membar_acquire>() )
                            return false ;
                        temp = ( temp + 1 ) & nModulo ;
                        tt = m_Array[ temp ] ;
                    }

                    if ( th != m_nHead.load<membar_acquire>() )
                        continue ;
                    // check whether the queue is empty
                    if ( temp == m_nTail.load<membar_acquire>() ) {
                        // help the enqueue to update end
                        m_nTail.cas<membar_release>( temp, (temp + 1) & nModulo ) ;
                        continue ;
                    }

                    pNull = cds::details::isMarkedBit( tt ) ? free0() : free1() ;

                    if ( th != m_nHead.load<membar_acquire>() )
                        continue ;

                    // Get the actual head, null means empty
                    if ( atomics::cas<membar_release>( m_Array + temp, tt, pNull )) {
                        if ( temp % 2 == 0 )
                            m_nHead.cas<membar_release>( th, temp ) ;
                        dest = cds::details::unmarkBit( tt ) ;
                        --m_ItemCounter    ;
                        return true ;
                    }

                TryAgain: ;
                } while ( bkoff(), true ) ;

                // No control path reaches this line!
                return false    ;
            }

            bool push( T *  data )        { return enqueue( data ); }
            bool pop( T *& data )        { return dequeue( data ); }

            bool empty() const            { return m_nTail.load<membar_acquire>() == ((m_nHead.load<membar_acquire>() + 1) & m_nCapacity) ; }

            size_t clear()
            {
                return generic_clear( *this )   ;
            }

            /// Return number of items in queue. Valid only if \p TRAITS::item_counter_type is not the cds::atomics::empty_item_counter
            size_t    size() const
            {
                return m_ItemCounter    ;
            }
        } ;

        /// Lock-free cyclic queue
        /**
            CAS-based queue based on cyclic array. The upper bound of items stored in the queue is limited.

            Source:
                \li [2000] Philippas Tsigas, Yi Zhang "A Simple, Fast and Scalable Non-Blocking Concurrent FIFO Queue
                for Shared Memory Multiprocessor Systems"

            Template arguments:
                \li \p T is type of value stored in the queue
                \li \p TRAITS is queue's traits class; default is cds::queue::traits.
                \li \p ALLOCATOR is memory allocator. Default is CDS_DEFAULT_ALLOCATOR
        */
        template < typename T,
            class TRAITS,
            class ALLOCATOR >
        class TZCyclicQueue: public concept::bounded_container
        {
        public:
            typedef T            value_type    ;
            typedef normalized_type_traits<TRAITS>      type_traits ;

        protected:
            TZCyclicQueue< T *, type_traits >    m_Queue    ;
            cds::details::Allocator< T, ALLOCATOR > m_Alloc ;

        private:
            T * allocNode( const T& data )
            {
                return m_Alloc.New( data )    ;
            }

            void freeNode( T * pNode )
            {
                m_Alloc.Delete( pNode )    ;
            }

        public:
            /// Constructs queue
            /**
                The capacity of cyclic array is 2**nPow2, nPow2 <= 31
            */
            TZCyclicQueue( unsigned int nPow2 = 20 )
                : m_Queue( nPow2 )
            {}
            ~TZCyclicQueue()
            {
                clear() ;
            }

            /// Enqueues \p data in lock-free manner
            /**
                Since the queue is based on an array and has upper limit of items contained,
                the method may return \a false if queue is full
            */
            bool enqueue( const T& newData )
            {
                T * p = allocNode( newData ) ;
                bool bRet = m_Queue.enqueue( p ) ;
                if ( !bRet )
                    freeNode( p ) ;
                return bRet ;
            }

               /** Dequeues a value to \p dest.
                If queue is empty returns \a false, \p dest is not changed.
                If queue is not empty returns \a true, \p dest contains the value dequeued
            */
            bool dequeue( T& data )
            {
                T * p ;
                if ( m_Queue.dequeue( p ) ) {
                    data = *p        ;
                    freeNode( p )    ;
                    return true        ;
                }
                return false    ;
            }

            /// Synonym for @ref enqueue
            bool push( const T& data )        { return enqueue( data ); }

            /// Synonym for @ref dequeue
            bool pop( T& data )                { return dequeue( data ); }

            /// Checks if the queue is empty
            bool empty() const                { return m_Queue.empty();  }

            /// Clears the queue
            size_t clear()
            {
                return generic_clear( *this )   ;
            }

            /// Returns number of items in queue. Valid only if \p TRAITS::item_counter_type is not the cds::atomics::empty_item_counter
            size_t    size() const
            {
                return m_Queue.size()    ;
            }

        };
    } // namespace queue
}    // namespace mx
//@endcond

#endif    // #ifndef __CDS_QUEUE_TZ_CYCLIC_QUEUE_H
