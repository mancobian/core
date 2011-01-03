/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_GC_TAGGED_DETAILS_FREE_LIST_H
#define __CDS_GC_TAGGED_DETAILS_FREE_LIST_H

/*
    Editions:
        2007.03.03  Maxim.Khiszinsky    Created
        2008.10.01    Maxim.Khiszinsky    Refactoring
*/

#include <cds/backoff_strategy.h>
#include <cds/details/aligned_allocator.h>

#ifdef CDS_DWORD_CAS_SUPPORTED

namespace cds { namespace gc { namespace tagged {

    /// FreeList of Tagged pointer reclamation schema
    /**
        FreeList is list of free nodes. To prevent ABA-problem tagged reclamation schema implies
        that the node is never freed physically. Instead of deallocation the node is placed in special container
        named free-list. So free-list may be considered as an allocator for data of type \p T.
        The implementation of free-list is canonical stack.

        \par Template parameters:
            \li \p T - type of node
            \li \p BACKOFF - back-off cshema. The default is backoff::empty
            \li \p ALLOCATOR - aligned memory allocator. The default class is defined by CDS_DEFAULT_ALIGNED_ALLOCATOR macro

        \par
        The main problem of tagged reclamation schema is destroying a node. The node of tagged containers
        is the structure like:
        \code
            template <typename T>
            struct Node {
                tagged_type< Node<T>* >    m_next    ;
                T                        m_data    ;
            }
        \endcode
        The free-list uses first sizeof(void*) bytes of \p T to link items to list.

        When the node is excluding from container it must be placed into appropriate free-list. However,
        on the destroying time the destructor of type T must not be called because of the access
        to deleted node can be possible (the free-list must be a type-stable container). So the destructor T::~T
        of data may be safely called in FreeList::pop method \b before returning the node cached in free-list.

        Different implementations of free-list may use a synthetic pointer instead of the pointer to node when allocating.
        For example, the pointer returned by \p alloc may be marked when some low bit(s) of the pointer
        is used by the free-list as a mark for internal purposes. Therefore, the methods of free-list implementations
        are get/return the type \p value_ptr instead of the pointer to \p T. In general case, \p value_ptr is not equal
        to \p T*. To get \p T* from \p value_ptr you must call \p dereference function. The data structures
        must store \p value_ptr as a pointer to node and they must use \p dereference function to get the pointer to the node
        from \p value_ptr.
    */
    template <typename T,
        class BACKOFF = backoff::empty
        ,class ALLOCATOR = CDS_DEFAULT_ALIGNED_ALLOCATOR
    >
    class FreeList
    {
        typedef cds::details::AlignedAllocator< T, ALLOCATOR > aligned_allocator    ;

    public:
        typedef    T        value_type    ;            ///< type of node
        typedef T *        value_ptr    ;            ///< type of pointer to node
        typedef BACKOFF backoff_strategy    ;   ///< Back-off strategy
        typedef typename aligned_allocator::allocator_type      allocator_type  ;   ///< Aligned allocator

        /// To support rebind to type \p Q, and back-off strategy \p B, and allocator \p A
        template <typename Q, typename B = BACKOFF, typename A = ALLOCATOR >
        struct rebind {
            typedef FreeList< Q, B, A >    other ;
        };

    private:
        /// Node of free-list.
        struct Node {
            Node *    m_pNext    ;
        }    ;

        typedef tagged_type< value_ptr >                tagged_node    ;    ///< Tagged node

        tagged_node            m_Top   ;   ///< top of the stack of free nodes
        aligned_allocator   m_Alloc    ;    ///< aligned allocator

    private:
        /// Places the node \p node to internal stack
        void push( T * node )
        {
            assert( cds::details::is_aligned<CDS_TAGGED_ALIGNMENT>( node ) )   ;

            BACKOFF backoff    ;
            tagged_node t    ;
            while ( true ) {
                t = atomics::load<membar_acquire>( &m_Top )    ;
                reinterpret_cast< Node *>( node )->m_pNext = reinterpret_cast< Node *>( t.data() ) ;
                if ( cas_tagged<membar_release>( m_Top, t, reinterpret_cast<value_ptr>( node )))
                    break        ;
                backoff();
            }
        }

        /** Pops cached node from internal stack. If free-list is not empty this method calls \p destroyData of
            popped node before return.
        */
        T * pop()
        {
            tagged_node t    ;
            BACKOFF backoff    ;
            while ( true ) {
                t = atomics::load<membar_acquire>( &m_Top )    ;
                if ( t.data() == NULL )
                    return NULL            ;
                if ( cas_tagged<membar_release>( m_Top, t, reinterpret_cast< value_ptr >( reinterpret_cast<Node *>( t.data() )->m_pNext ) ))
                    break    ;
                backoff();
            }
            assert( t.m_data != NULL )    ;
            T * p = reinterpret_cast< T *>( t.data() )    ;
            assert( cds::details::is_aligned<CDS_TAGGED_ALIGNMENT>( p ) )   ;
            p->destroyData()    ;
            return p        ;
        }

        /// Clears internal stack. This method physically deletes all cached nodes.
        void clear()
        {
            tagged_node t    ;
            do {
                t = atomics::load<membar_acquire>( &m_Top ) ;
                if ( t.data() == NULL )
                    break    ;
            } while ( !cas_tagged<membar_release>( m_Top, t, (T *) NULL ) )    ;

            // t contains the list's head and it is private for current thread
            Node * p = reinterpret_cast<Node *>( t.data() )    ;
            while ( p ) {
                Node * pNext = p->m_pNext    ;
                m_Alloc.Delete( reinterpret_cast< T *>( p ))    ;
                p = pNext    ;
            }
        }

    public:
        FreeList()
        {
            CDS_STATIC_ASSERT( sizeof(T) >= sizeof(void *) )    ;
        }

        ~FreeList()
        {
            clear()    ;
        }

        /// Get new node from free-list. If free-list is empty the new node is allocated
        /**
            The function returns \p value_ptr value. In general case, it is not equal to pointer to node. To get \p T* from
            \p value_ptr you must call \p dereference function.
        */
        value_ptr    alloc()
        {
            T * pNew = pop()    ;
            if ( pNew )
                pNew->constructData()    ;
            else {
                pNew = m_Alloc.New( CDS_TAGGED_ALIGNMENT )    ;
                assert( cds::details::is_aligned<CDS_TAGGED_ALIGNMENT>( pNew ) )   ;
            }
            return pNew ;
        }

        /// Get new node from free-list and call copy constructor. If free-list is empty the new node is allocated
        template <typename T1>
        value_ptr    alloc( const T1& init)
        {
            T * pNew = pop()    ;
            if ( pNew ) {
                assert( cds::details::is_aligned<CDS_TAGGED_ALIGNMENT>( pNew ) )   ;
                pNew->constructData( init )    ;
            }
            else {
                pNew = m_Alloc.New( CDS_TAGGED_ALIGNMENT, init )    ;
                assert( cds::details::is_aligned<CDS_TAGGED_ALIGNMENT>( pNew ) )   ;
            }
            //assert( cds::details::is_aligned<CDS_TAGGED_ALIGNMENT>( pNew ) )   ;
            return pNew ;
        }

        /// Get new node from free-list and call copy constructor. If free-list is empty the new node is allocated
        template <typename T1, typename T2>
        value_ptr    alloc( const T1& init1, const T2& init2)
        {
            T * pNew = pop()    ;
            if ( pNew ) {
                assert( cds::details::is_aligned<CDS_TAGGED_ALIGNMENT>( pNew ) )   ;
                pNew->constructData( init1, init2 )    ;
            }
            else {
                pNew = m_Alloc.New( CDS_TAGGED_ALIGNMENT, init1, init2 )    ;
                assert( cds::details::is_aligned<CDS_TAGGED_ALIGNMENT>( pNew ) )   ;
            }
            //assert( cds::details::is_aligned<CDS_TAGGED_ALIGNMENT>( pNew ) )   ;
            return pNew ;
        }

        /// Place node \p p to free-list. The node is not physically destructed
        void free( value_ptr p )
        {
            assert( cds::details::is_aligned<CDS_TAGGED_ALIGNMENT>( p ) )   ;
            push( p )    ;
        }

        /// Returns pointer to \p T extracted from \p ptr
        /**
            Some implementations of free-list use a synthetic pointer instead of the pointer to node when allocating.
            For example, the pointer \p alloc returned may be marked when some low bit(s) of the pointer
            is used by the free-list as a mark for internal purposes.
            This function converts free-list synthetic pointer to common pointer to the node that can be used safely by
            application.
        */
        T * dereference( value_ptr ptr )    { return ptr; }
    };
}}} // namespace cds::gc::tagged

#endif // #ifdef CDS_DWORD_CAS_SUPPORTED

#endif    // #ifndef __CDS_GC_TAGGED_DETAILS_FREE_LIST_H
