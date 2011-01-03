/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_GC_HZP_HAZARDPTR_H
#define __CDS_GC_HZP_HAZARDPTR_H

#include <cds/gc/hzp_gc.h>
#include <cds/atomic/atomic.h>
#include <cds/os/thread.h>

#include <cds/gc/hzp/details/hp_fwd.h>
#include <cds/gc/hzp/details/hp_alloc.h>
#include <cds/gc/hzp/details/hp_retired.h>

#include <vector>
#include <boost/noncopyable.hpp>

/*
    Editions:
        2007.12.24    cds    Add statistics and CDS_GATHER_HAZARDPTR_STAT macro
        2008.03.06  cds Refactoring: implementation of HazardPtrMgr is moved to hazardptr.cpp
        2008.03.08  cds    Remove HazardPtrMgr singleton. Now you must initialize/destroy HazardPtrMgr calling
                        HazardPtrMgr::Construct / HazardPtrMgr::Destruct before use (usually in main() function).
        2008.12.06  cds Refactoring. Changes class name, namespace hierarchy, all helper defs have been moved to details namespace
        2010.01.27  cds Introducing memory order constraint
*/

namespace cds {
    namespace gc {

    /// Michael's Hazard Pointers reclamation schema
    /**
    Michael's Hazard Pointers safe reclamation schema

    \par Sources:
        \li [2002] Maged M.Michael "Safe memory reclamation for dynamic lock-freeobjects using atomic reads and writes"
        \li [2003] Maged M.Michael "Hazard Pointers: Safe memory reclamation for lock-free objects"
        \li [2004] Andrei Alexandrescy, Maged Michael "Lock-free Data Structures with Hazard Pointers"

    \par Usage:
        Hazard Pointer garbage collector (GC) is the singleton. Before use any HP-related class you must initialize
        HP GC by call cds::gc::hzp::GarbageCollector::Construct. We recommend to do it in beginning of your main(). Before stop your
        application you must call cds::gc::hzp::GarbageCollector::Destruct() to destroy HP GC singleton:
        \code
        #include <cds/gc/hzp/hzp.h>

        int main(int argc, char** argv) {

            // Init HP GC
            cds::gc::hzp::GarbageCollector::Construct()    ;

            // Start your thread
            create_thread( myThreadEntryPoint )  ;

            // Do some work, for example, wait while thread running
            ...

            // Destruct HP GC
            cds::gc::hzp::GarbageCollector::Destruct()    ;

            return 0    ;
        }
        \endcode

        The HZP GC provides only core functionality that is not intended for direct use in your code. Instead,
        each thread works with cds::gc::hzp::ThreadGC object that is middle layer between HZP GC kernel and your
        thread. Before applying a HZP-based container, the ThreadGC object must be created and initialized properly
        by calling cds::threading ::Manager::attachThread() function in beginning of your thread.
        Before terminating the thread must call cds::threading ::Manager::detachThread() function:
        \code
        #include <cds/threading/model.h>

        // Thread function
        int myThreadEntryPoint()
        {
            // Attach current thread to GC used
            // Note, there is only one attachThread call.
            // The CDS library will properly initialize each GC constructed by you in single attachThread call
            cds::threading::Manager::attachThread()

            // Do thread's work
            ...

            // Detach current thread from GC used
            // Note, there is only one detachThread call
            cds::threading::Manager::detachThread()

            return 0    ;
        }
        \endcode
    */
    namespace hzp {

        namespace details {
            /// Hazard pointer record of the thread
            /**
                The structure of type "single writer - multiple reader": only the owner thread may write to this structure
                other threads have read-only access.
            */
            struct HPRec {
                HPAllocator<HazardPtr>    m_hzp         ; ///< array of hazard pointers. Implicit CDS_DEFAULT_ALLOCATOR dependency
                retired_vector            m_arrRetired ; ///< Retired pointer array

                /// Ctor
                HPRec( const cds::gc::hzp::GarbageCollector& HzpMgr ) ;    // inline
                ~HPRec()
                {}

                /// Clears all hazard pointers
                void clear()
                {
                    m_hzp.clear()    ;
                }
            };
        }    // namespace details

        /// Hazard Pointer singleton
        /**
            Safe memory reclamation schema by Michael "Hazard Pointers"

        \par Sources:
            \li [2002] Maged M.Michael "Safe memory reclamation for dynamic lock-freeobjects using atomic reads and writes"
            \li [2003] Maged M.Michael "Hazard Pointers: Safe memory reclamation for lock-free objects"
            \li [2004] Andrei Alexandrescy, Maged Michael "Lock-free Data Structures with Hazard Pointers"

        */
        class CDS_API_CLASS GarbageCollector
        {
        public:
            /// Internal GC statistics
            struct InternalState {
                size_t              nHPCount                ;   ///< HP count per thread (const)
                size_t              nMaxThreadCount         ;   ///< Max thread count (const)
                size_t              nMaxRetiredPtrCount     ;   ///< Max retired pointer count per thread (const)
                size_t              nHPRecSize              ;   ///< Size of HP record, bytes (const)

                size_t              nHPRecAllocated         ;   ///< Count of HP record allocations
                size_t              nHPRecUsed              ;   ///< Count of HP record used
                size_t              nTotalRetiredPtrCount   ;   ///< Current total count of retired pointers
                size_t              nRetiredPtrInFreeHPRecs ;   ///< Count of retired pointer in free (unused) HP records

                atomics::event_counter::value_type  evcAllocHPRec   ;   ///< Count of HPRec allocations
                atomics::event_counter::value_type  evcRetireHPRec  ;   ///< Count of HPRec retire events
                atomics::event_counter::value_type  evcAllocNewHPRec;   ///< Count of new HPRec allocations from heap
                atomics::event_counter::value_type  evcDeleteHPRec  ;   ///< Count of HPRec deletions

                atomics::event_counter::value_type  evcScanCall     ;   ///< Count of Scan calling
                atomics::event_counter::value_type  evcHelpScanCall ;   ///< Count of HelpScan calling
                atomics::event_counter::value_type  evcScanFromHelpScan;///< Count of Scan calls from HelpScan

                atomics::event_counter::value_type  evcDeletedNode  ;   ///< Count of deleting of retired objects
                atomics::event_counter::value_type  evcDeferredNode ;   ///< Count of objects that cannot be deleted in Scan phase because of a HazardPtr guards it
            } ;

            /// No GarbageCollector object is created
            CDS_DECLARE_EXCEPTION( HZPManagerEmpty, "Global Hazard Pointer GarbageCollector is NULL" )    ;

            /// Not enough required Hazard Pointer count
            CDS_DECLARE_EXCEPTION( HZPTooMany, "Not enough required Hazard Pointer count" )    ;

        private:
            /// Internal GC statistics
            struct Statistics {
                atomics::event_counter  m_AllocHPRec            ;    ///< Count of HPRec allocations
                atomics::event_counter  m_RetireHPRec            ;    ///< Count of HPRec retire events
                atomics::event_counter  m_AllocNewHPRec            ;    ///< Count of new HPRec allocations from heap
                atomics::event_counter  m_DeleteHPRec            ;    ///< Count of HPRec deletions

                atomics::event_counter  m_ScanCallCount            ;    ///< Count of Scan calling
                atomics::event_counter  m_HelpScanCallCount        ;    ///< Count of HelpScan calling
                atomics::event_counter  m_CallScanFromHelpScan    ;    ///< Count of Scan calls from HelpScan

                atomics::event_counter  m_DeletedNode            ;    ///< Count of retired objects deleting
                atomics::event_counter  m_DeferredNode            ;    ///< Count of objects that cannot be deleted in Scan phase because of a HazardPtr guards it
            };

            /// Internal list of cds::gc::hzp::details::HPRec
            struct hplist_node: public details::HPRec
            {
                atomic<hplist_node *> m_pNext     ; ///< next hazard ptr record in list
                atomic<OS::ThreadId>        m_idOwner   ; ///< Owner thread id; 0 - the record is free (not owned)
                atomic<bool>                m_bFree     ; ///< true if record if free (not owned)

                //@cond
                hplist_node( const GarbageCollector& HzpMgr )
                    : HPRec( HzpMgr ),
                    m_pNext(NULL),
                    m_idOwner( OS::nullThreadId() ),
                    m_bFree( true )
                {}

                ~hplist_node()
                {
                    assert( m_idOwner.load<membar_relaxed>() == OS::nullThreadId() )    ;
                    assert( m_bFree.load<membar_relaxed>() )    ;
                }
                //@endcond
            };


            hplist_node *     m_pListHead  ;            ///< Head of GC list

            static GarbageCollector *    m_pHZPManager    ;    ///< GC instance pointer

            Statistics                m_Stat    ;                ///< Internal statistics
            bool                    m_bStatEnabled    ;        ///< true - statistics enabled

            const size_t            m_nHazardPointerCount    ;    ///< max count of thread's hazard pointer
            const size_t            m_nMaxThreadCount        ;    ///< max count of thread
            const size_t            m_nMaxRetiredPtrCount    ;    ///< max count of retired ptr per thread

        private:
            /// Ctor
            GarbageCollector(
                size_t nHazardPtrCount = 0,            ///< Hazard pointer count per thread
                size_t nMaxThreadCount = 0,            ///< Max count of thread
                size_t nMaxRetiredPtrCount = 0        ///< Capacity of the array of retired objects
            )    ;

            /// Dtor
            ~GarbageCollector()    ;

            /// Allocate new HP record
            hplist_node * NewHPRec()    ;

            /// Permanently deletes HPrecord \p pNode
            /**
                Caveat: for performance reason this function is defined as inline and cannot be called directly
            */
            void                DeleteHPRec( hplist_node * pNode );

            /// Permanently deletes retired pointer \p p
            /**
                Caveat: for performance reason this function is defined as inline and cannot be called directly
            */
            void                DeletePtr( details::retired_ptr& p );

        public:
            /// Creates GarbageCollector singleton
            /**
                GC is the singleton. If GC instance is not exist then the function creates the instance.
                Otherwise it does nothing.

                The Michael's HP reclamation schema depends of three parameters:

                \p nHazardPtrCount - HP pointer count per thread. Usually it is small number (2-4) depending from
                                     the data structure algorithms. By default, if \p nHazardPtrCount = 0,
                                     the function uses maximum of HP count for CDS library.

                \p nMaxThreadCount - max count of thread with using HP GC in your application. Default is 100.

                \p nMaxRetiredPtrCount - capacity of array of retired pointers for each thread. Must be greater than
                                    \p nHazardPtrCount * \p nMaxThreadCount.
                                    Default is 2 * \p nHazardPtrCount * \p nMaxThreadCount.
            */
            static void    CDS_STDCALL Construct(
                size_t nHazardPtrCount = 0,            ///< Hazard pointer count per thread
                size_t nMaxThreadCount = 0,            ///< Max count of thread in your application
                size_t nMaxRetiredPtrCount = 0        ///< Capacity of the array of retired objects for the thread
            );

            /// Destroys global instance of GarbageCollector
            static void CDS_STDCALL Destruct()    ;

            /// Returns pointer to GarbageCollector instance
            static GarbageCollector&   instance()
            {
                if ( m_pHZPManager == NULL )
                    throw HZPManagerEmpty()    ;
                return *m_pHZPManager   ;
            }

            /// Checks if global GC object is constructed and may be used
            static bool isUsed()
            {
                return m_pHZPManager != NULL    ;
            }

            /// Returns max Hazard Pointer count defined in construction time
            size_t            getHazardPointerCount() const        { return m_nHazardPointerCount; }

            /// Returns max thread count defined in construction time
            size_t            getMaxThreadCount() const             { return m_nMaxThreadCount; }

            /// Returns max size of retired objects array. It is defined in construction time
            size_t            getMaxRetiredPtrCount() const        { return m_nMaxRetiredPtrCount; }

            // Internal statistics

            /// Get internal statistics
            InternalState& getInternalState(InternalState& stat) const    ;

            /// Checks if internal statistics enabled
            bool              isStatisticsEnabled() const { return m_bStatEnabled; }

            /// Enables/disables internal statistics
            bool              enableStatistics( bool bEnable )
            {
                bool bEnabled = m_bStatEnabled    ;
                m_bStatEnabled = bEnable        ;
                return bEnabled                    ;
            }

            /// Checks that required hazard pointer count \p nRequiredCount is less or equal then max hazard pointer count
            /**
                If \p nRequiredCount > getHazardPointerCount() then the exception HZPTooMany is thrown
            */
            static void checkHPCount( unsigned int nRequiredCount )
            {
                if ( instance().getHazardPointerCount() < nRequiredCount )
                    throw HZPTooMany()  ;
            }

        public:    // Internals for threads

            /// Allocates Hazard Pointer GC record. For internal use only
            details::HPRec * AllocateHPRec()    ;

            /// Free HP record. For internal use only
            void RetireHPRec( details::HPRec * pRec )    ;

            /// The main garbage collecting function
            /**
                A scan includes four stages. The first stage involves scanning the array HP for non-null values.
                Whenever a non-null value is encountered, it is inserted in a local list of currently protected pointer.
                Only stage 1 accesses shared variables. The following stages operate only on private variables.

                The second stage of a scan involves sorting local list of protected pointers to allow
                binary search in the third stage.

                The third stage of a scan involves checking each reclaimed node
                against the pointers in local list of protected pointers. If the binary search yields
                no match, the node is freed. Otherwise, it cannot be deleted now and must kept in thread's list
                of reclaimed pointers.

                The forth stage prepares new thread's private list of reclaimed pointers
                that could not be freed during the current scan, where they remain until the next scan.

                This function is called internally by ThreadGC object when upper bound of thread's list of reclaimed pointers
                is reached.
            */
            void Scan( details::HPRec * pRec )    ;

            /// Helper scan routine
            /**
                The function guarantees that every node that is eligible for reuse is eventually freed, barring
                thread failures. To do so, after executing Scan, a thread executes a HelpScan,
                where it checks every HP record. If an HP record is inactive, the thread moves all "lost" reclaimed pointers
                to thread's list of reclaimed pointers.

                The function is called internally by Scan.
            */
            void HelpScan( details::HPRec * pThis ) ;
        };

        /// Thread's hazard pointer manager
        /**
            To use Hazard Pointer reclamation schema each thread object must be linked with the object of ThreadGC class
            that interacts with GarbageCollector global object. The linkage is performed by calling cds::threading \p ::Manager::attachThread()
            on the start of each thread that uses HP GC. Before terminating the thread linked to HP GC it is necessary to call
            cds::threading \p ::Manager::detachThread().
        */
        class ThreadGC: boost::noncopyable
        {
            GarbageCollector&    m_HzpManager    ;    ///< Hazard Pointer GC singleton
            details::HPRec *    m_pHzpRec        ;    ///< Pointer to thread's HZP record

        public:
            ThreadGC()
                : m_HzpManager( GarbageCollector::instance() ),
                m_pHzpRec( NULL )
            {}
            ~ThreadGC()
            {
                fini()    ;
            }

            /// Checks if thread GC is initialized
            bool    isInitialized() const   { return m_pHzpRec != NULL ; }

            /// Initialization. Repeat call is available
            void init()
            {
                if ( !m_pHzpRec )
                    m_pHzpRec = m_HzpManager.AllocateHPRec() ;
            }

            /// Finalization. Repeat call is available
            void fini()
            {
                if ( m_pHzpRec ) {
                    details::HPRec * pRec = m_pHzpRec    ;
                    m_pHzpRec = NULL    ;
                    m_HzpManager.RetireHPRec( pRec ) ;
                }
            }

            /// Initializes HP guard \p guard
            void allocGuard( details::HPGuard& guard )
            {
                assert( m_pHzpRec != NULL )    ;
                m_pHzpRec->m_hzp.alloc( guard )    ;
            }

            /// Frees HP guard \p guard
            void freeGuard( details::HPGuard& guard )
            {
                assert( m_pHzpRec != NULL )    ;
                m_pHzpRec->m_hzp.free( guard )    ;
            }

            /// Initializes HP guard array \p arr
            template <size_t COUNT>
            void allocGuard( details::HPArray<COUNT>& arr )
            {
                assert( m_pHzpRec != NULL )    ;
                m_pHzpRec->m_hzp.alloc( arr )    ;
            }

            /// Frees HP guard array \p arr
            template <size_t COUNT>
            void freeGuard( details::HPArray<COUNT>& arr )
            {
                assert( m_pHzpRec != NULL )    ;
                m_pHzpRec->m_hzp.free( arr )    ;
            }

            /// Places retired pointer \p and its deleter \p pFunc into thread's array of retired pointer for deferred reclamation
            template <typename T>
            void retirePtr( T * p, void (* pFunc)(T *) )
            {
                retirePtr( details::retired_ptr( reinterpret_cast<void *>( p ), reinterpret_cast<free_retired_ptr_func>( pFunc ) ) )    ;
            }

            /// Places retired pointer \p into thread's array of retired pointer for deferred reclamation
            void retirePtr( const details::retired_ptr& p )
            {
                m_pHzpRec->m_arrRetired.push( p ) ;

                if ( m_pHzpRec->m_arrRetired.isFull() ) {
                    // Max of retired pointer count is reached. Do scan
                    m_HzpManager.Scan( m_pHzpRec )     ;
                    m_HzpManager.HelpScan( m_pHzpRec ) ;
                }
            }

            /// Safely de-references \p ppNode link
            /**
                Safely de-references \p ppNode link and sets \p hp hazard pointer to the de-referenced node
                \p TNODE - class derived from ContainerNode type
            */
            template <typename TNODE>
            static TNODE *    derefLink( TNODE * volatile * ppNode, HazardPtr& hp )
            {
                TNODE * p    ;
                while ( true ) {
                    p = atomics::load<membar_relaxed>( ppNode ) ;
                    atomics::store<membar_release>( &hp, reinterpret_cast<HazardPtr>(p) )    ;
                    if ( atomics::load<membar_acquire>( ppNode ) == p )
                        break    ;
                }
                return p    ;
            }

            /// Safely de-references \p ppNode link
            /**
                Safely de-references \p ppNode link and sets \p hp hazard pointer to the de-referenced node
                \p TNODE - class derived from ContainerNode type
            */
            template <typename TNODE>
            static TNODE *    derefLink( TNODE * volatile * ppNode, details::HPGuard& hp )
            {
                return derefLink( ppNode, hp.getHPRef() )   ;
            }

        };

        /// Auto HPGuard.
        /**
            This class incapsulates Hazard Pointer guard to protect a pointer against deletion .
            It allocates one HP from thread's HP array in constructor and free the HP allocated in destruction time.
        */
        class AutoHPGuard: public details::HPGuard
        {
            //@cond
            typedef details::HPGuard base_class ;
            ThreadGC&    m_gc    ;    ///< Thread GC
            //@endcond

        public:
            /// Allocates HP guard from \p gc
            AutoHPGuard( ThreadGC& gc )
                : m_gc( gc )
            {
                gc.allocGuard( *this )    ;
            }

            /// Allocates HP guard from \p gc and protects the pointer \p p of type \p T
            template <typename T>
            AutoHPGuard( ThreadGC& gc, T * p  )
                : m_gc( gc )
            {
                gc.allocGuard( *this )    ;
                *this = p               ;
            }

            /// Frees HP guard. The pointer guarded may be deleted after this.
            ~AutoHPGuard()
            {
                m_gc.freeGuard( *this )    ;
            }

            /// Returns thread GC
            ThreadGC&    getGC() const { return m_gc; }

            /// Protects the pointer \p p against reclamation (guards the pointer).
            template <typename T>
            T * operator =( T * p )
            {
                return base_class::operator =( p )    ;
            }
        };

        /// Auto-managed array of hazard pointers
        /**
            This class is wrapper around cds::gc::hzp::details::HPArray class.
            \p COUNT is the size of HP array
        */
        template <size_t COUNT>
        class AutoHPArray: public details::HPArray<COUNT>
        {
            ThreadGC&    m_mgr    ;    ///< Thread GC

        public:
            /// Rebind array for other size \p COUNT2
            template <size_t COUNT2>
            struct rebind {
                typedef AutoHPArray<COUNT2>  other   ;   ///< rebinding result
            };

        public:
            /// Allocates array of HP guard from \p mgr
            AutoHPArray( ThreadGC& mgr )
                : m_mgr( mgr )
            {
                mgr.allocGuard( *this )    ;
            }

            /// Frees array of HP guard
            ~AutoHPArray()
            {
                m_mgr.freeGuard( *this )    ;
            }

            /// Returns thread GC
            ThreadGC&    getGC() const { return m_mgr; }
        };

    }   // namespace hzp

} /*  namespace gc */ }   /* namespace cds */

// Inlines
#include <cds/gc/hzp/details/hp_inline.h>

#endif  // #ifndef __CDS_GC_HZP_HAZARDPTR_H
