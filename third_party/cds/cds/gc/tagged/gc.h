/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_GC_TAGGED_GC_IMPL_H
#define __CDS_GC_TAGGED_GC_IMPL_H

/*
    Editions:
        2010.02.22  Maxim.Khiszinsky    FreeList implementation is moved to separate header (details/free_list.h)
        2008.10.01  Maxim.Khiszinsky    Refactoring
        2007.03.03  Maxim.Khiszinsky    Created
*/

#include <cds/gc/tagged_gc.h>
#include <cds/atomic/tagged_ptr.h>
#include <cds/details/allocator.h>

namespace cds { namespace gc {

    /// Tagged pointer reclamation schema to solve ABA-problem
    /**
        Tagged pointer reclamation schema was founded by IBM for resolving ABA problem
        of CAS-based algorithms.
        Each CASable pointer in this schema consists of two part: the pointer and the tag.
        The CAS operation atomically increments the tag any time when CAS is succeeded.
        In practice, it is means that the goal platform must support double-word CAS primitive.
        Tagged pointer is sufficiently efficient technique; unfortunately, not all of modern CPU
        architecture supports double-word CAS primitive; currently, only x86 and amd64 have
        full support of double-word CAS in 32-bit and 64-bit mode.

        The macro CDS_DWORD_CAS_SUPPORTED is defined for the architecture that supports double-word
        CAS.

    \par Implementation notes
        It seems the tagged GC is not safe for complex data structure.

        Consider the map based on tagged GC schema. Let's item's key is std::string 
        (i.e. dynamically allocated buffer).

        Thread F is seeking key K in the map. Suppose, it is preempted at the item with key K.
        Thread D deletes key K from the map. According to tagged GC, the item with key K places
        to the free-list. Notes, when a item is moved to the free-list the item's internal data
        (the string in our case) is not destroyed.

        At the same time, the thread X inserts the key K in the map. It allocates new item from 
        GC's free-list. Since the free-list is stack-based the item popped is the item that has 
        just been deleted by the thread D. When an item is popped from the free-list, the destructor 
        of internal data of the item must be called to clean it.
        Suppose, when item's destructor is processing, the thread F resumes and compares its current item 
        with the key K. Oops! The current item for thread F is the item that is being destroy at 
        this moment and item's key is K.  

        This case is one of typical problem of lock-free programming. For complex data structures,
        before an item can be reused we must be sure that no thread can access to it. The tagged
        GC saves us from ABA-problem but it does not solve "early reusing" problem.

        For simple data structures tagged GC is safe.
    */
    namespace tagged {

#    ifdef CDS_DWORD_CAS_SUPPORTED
        // The platform supports dword CAS primitive

        /// Tag type
        typedef uptr_atomic_t        ABA_tag    ;

        /// Tagged data
        template <typename T>
        struct CDS_TAGGED_ALIGN_ATTRIBUTE tagged_type {
            T volatile m_data           ;   ///< Data. Condition: sizeof(data) == sizeof(uptr_atomic_t)
            ABA_tag volatile m_nTag     ;   ///< ABA-prevention tag. It might not be decreased during lifetime.

            /// Default ctor. Initializes pointer to NULL and tag to 0.
            tagged_type()
                : m_data( 0 )
                , m_nTag(0)
            {
                CDS_STATIC_ASSERT( sizeof(T) == sizeof(uptr_atomic_t) )    ;
                CDS_STATIC_ASSERT(sizeof(tagged_type<T>) == 2 * sizeof(uptr_atomic_t)) ;
            }

            /// Constructor
            tagged_type(
                T data,         ///< data
                ABA_tag nTag    ///< Tag value
                )
                : m_data( data )
                , m_nTag( nTag )
            {
                CDS_STATIC_ASSERT( sizeof(T) == sizeof(uptr_atomic_t) )    ;
                CDS_STATIC_ASSERT(sizeof(tagged_type<T>) == 2 * sizeof(uptr_atomic_t)) ;
            }

            /// Copy constructor
            tagged_type( const tagged_type<T>& v )
                : m_data( v.m_data )
                , m_nTag( v.m_nTag )
            {}

            /// Assignment operator.
            tagged_type<T>&    operator =( const tagged_type<T>& src )
            {
                CDS_STATIC_ASSERT( sizeof(T) == sizeof(uptr_atomic_t) )    ;
                CDS_STATIC_ASSERT(sizeof(tagged_type<T>) == 2 * sizeof(uptr_atomic_t)) ;

                m_data = src.m_data    ;
                m_nTag = src.m_nTag    ;
                return *this        ;
            }

            /// Get value of tagged_type
            T  data()                { return const_cast<T>( m_data ); }            // drop volatile
            /// Get value of tagged_type
            T  data() const            { return const_cast<T>( m_data ); }            // drop volatile
            /// Get reference to tagged_type's value
            T& ref()                { return const_cast<T>( m_data ); }            // drop volatile

            /// Tagged data equality
            /**
                Tagged data is equal iff they data AND they tags are equal.
            */
            friend inline bool operator ==( const tagged_type<T>& r1, const tagged_type<T>& r2 )
            {
                return r1.m_data == r2.m_data && r1.m_nTag == r2.m_nTag    ;
            }

            //@cond
            friend inline bool operator !=( const tagged_type<T>& r1, const tagged_type<T>& r2 )
            {
                return !( r1 == r2 ) ;
            }
            //@endcond
        }  ;

        /// CAS specialization for tagged_type<T>. cas_tagged increments the counter of \p curVal on any call
        template <typename T>
        static inline bool cas_tagged( tagged_type<T> volatile & dest, const tagged_type<T>& curVal, T dataNew, memory_order success_order, memory_order failure_order )
        {
            CDS_STATIC_ASSERT( sizeof( tagged_type<T> ) == 2 * sizeof( void * ) )    ;
            assert( cds::details::is_aligned<CDS_TAGGED_ALIGNMENT>( &dest ) )   ;

            tagged_type<T> newVal( dataNew, curVal.m_nTag + 1 )    ;
            return atomics::cas( &dest, curVal, newVal, success_order, failure_order )  ;
        }

        /// CAS specialization for tagged_type<T>. cas_tagged increments the counter of \p curVal on any call
        template <typename ORDER, typename T>
        static inline bool cas_tagged( tagged_type<T> volatile & dest, const tagged_type<T>& curVal, T dataNew )
        {
            CDS_STATIC_ASSERT( sizeof( tagged_type<T> ) == 2 * sizeof( void * ) )    ;
            assert( cds::details::is_aligned<CDS_TAGGED_ALIGNMENT>( &dest ) )   ;

            tagged_type<T> newVal( dataNew, curVal.m_nTag + 1 )    ;
            return atomics::cas<ORDER>( &dest, curVal, newVal )  ;
        }


        /// Base for tagged container node
        /**
            \par Template parameters:
                \li \p T - type of data stored in container
                \li \p NODE - node class.

            \par
            It is not necessary that the implementation of the container node is derived from ContainerNode.
            To meet the FreeList reclamation schema the node class must provide the following interface:

            Destruct data:
            \code
                void destroyData()                        { m_data.T::~T(); }
            \endcode
            Construct data:
            \code
                void constructData()                    { new ( &m_data ) T; }
                void constructData( const T& src )        { new ( &m_data ) T( src ); }
            \endcode
        */
        template <typename T, class NODE>
        struct CDS_TAGGED_ALIGN_ATTRIBUTE ContainerNode {
            typedef tagged_type< NODE * >    tagged_ptr    ;   ///< type of pointer to node

            /// Get pointer to node
            T&        data()                        { return static_cast<NODE *>(this)->m_data; }
            //@cond
            void destroyData()                    { cds::details::call_dtor( &( data()) ) ; }
            void constructData()                { new ( &data() ) T; }
            void constructData(const T& src )    { new ( &data() ) T( src ); }
            //@endcond
        } ;

#    endif    // ifdef CDS_DWORD_CAS_SUPPORTED

    } // namespace tagged
}} // namespace cds::gc

#include <cds/gc/tagged/details/free_list.h>

#endif // #ifndef __CDS_GC_TAGGED_GC_IMPL_H
