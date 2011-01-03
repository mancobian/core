/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


/*
    File: hzp_gc.cpp

    Hazard Pointers memory reclamation strategy implementation

    Editions:
        2008.02.10    Maxim.Khiszinsky    Created
*/

#include <cds/gc/hzp/hzp.h>

#include <algorithm>    // std::sort
#include "hzp_const.h"

#define    CDS_HAZARDPTR_STATISTIC( _x )    if ( m_bStatEnabled ) { _x; }

namespace cds { namespace gc {
    namespace hzp {

        /// Max array size of retired pointers
        static const size_t c_nMaxRetireNodeCount = c_nHazardPointerPerThread * c_nMaxThreadCount * 2 ;

        GarbageCollector *    GarbageCollector::m_pHZPManager = NULL;

        void CDS_STDCALL GarbageCollector::Construct( size_t nHazardPtrCount, size_t nMaxThreadCount, size_t nMaxRetiredPtrCount )
        {
            if ( !m_pHZPManager ) {
                m_pHZPManager = new GarbageCollector( nHazardPtrCount, nMaxThreadCount, nMaxRetiredPtrCount )    ;
            }
        }

        void CDS_STDCALL GarbageCollector::Destruct()
        {
            if ( m_pHZPManager ) {
                delete m_pHZPManager    ;
                m_pHZPManager = NULL    ;
            }
        }

        GarbageCollector::GarbageCollector(    size_t nHazardPtrCount, size_t nMaxThreadCount, size_t nMaxRetiredPtrCount )
            : m_pListHead(NULL),
            m_bStatEnabled( true ),
            m_nHazardPointerCount( nHazardPtrCount == 0 ? c_nHazardPointerPerThread : nHazardPtrCount ),
            m_nMaxThreadCount( nMaxThreadCount == 0 ? c_nMaxThreadCount : nMaxThreadCount ),
            m_nMaxRetiredPtrCount( nMaxRetiredPtrCount > c_nMaxRetireNodeCount ? nMaxRetiredPtrCount : c_nMaxRetireNodeCount )
        {}

        GarbageCollector::~GarbageCollector()
        {
            CDS_DEBUG_DO( const cds::OS::ThreadId nullThreadId = cds::OS::nullThreadId() );
            CDS_DEBUG_DO( const cds::OS::ThreadId mainThreadId = cds::OS::getCurrentThreadId() );

            //assert( mainThreadId == cds::OS::getMainThreadId() )    ;
            hplist_node * pHead = m_pListHead ;
            m_pListHead = NULL  ;
            hplist_node * pNext = NULL    ;
            for ( hplist_node * hprec = pHead; hprec; hprec = pNext ) {
                assert( hprec->m_idOwner.load<membar_relaxed>() == nullThreadId || hprec->m_idOwner.load<membar_relaxed>() == mainThreadId )    ;
                details::retired_vector& vect = hprec->m_arrRetired    ;
                details::retired_vector::iterator itRetired = vect.begin()    ;
                details::retired_vector::iterator itRetiredEnd = vect.end();
                while ( itRetired != itRetiredEnd ) {
                    DeletePtr( *itRetired )    ;
                    ++itRetired    ;
                }
                vect.clear()    ;
                pNext = hprec->m_pNext.load<membar_relaxed>() ;
                hprec->m_bFree.store<membar_relaxed>( true )  ;
                DeleteHPRec( hprec )    ;
            }
        }

        inline GarbageCollector::hplist_node * GarbageCollector::NewHPRec()
        {
            CDS_HAZARDPTR_STATISTIC( ++m_Stat.m_AllocNewHPRec )    ;
            return new hplist_node( *this ) ;
        }

        inline void GarbageCollector::DeleteHPRec( hplist_node * pNode )
        {
            CDS_HAZARDPTR_STATISTIC( ++m_Stat.m_DeleteHPRec )    ;
            assert( pNode->m_arrRetired.size() == 0 )    ;
            delete pNode    ;
        }

        inline void GarbageCollector::DeletePtr( details::retired_ptr& p )
        {
            CDS_HAZARDPTR_STATISTIC( ++m_Stat.m_DeletedNode )    ;
            p.free()    ;
        }

        details::HPRec * GarbageCollector::AllocateHPRec()
        {
            CDS_HAZARDPTR_STATISTIC( ++m_Stat.m_AllocHPRec )    ;

            hplist_node * hprec   ;
            const cds::OS::ThreadId nullThreadId = cds::OS::nullThreadId() ;
            const cds::OS::ThreadId curThreadId  = cds::OS::getCurrentThreadId()    ;
            // First try to reuse a retired (non-active) HP record
            for ( hprec = atomics::load<membar_acquire>( &m_pListHead ); hprec; hprec = hprec->m_pNext.load<membar_acquire>() ) {
                if ( !hprec->m_idOwner.cas<membar_seq_cst>( nullThreadId, curThreadId ) )
                    continue    ;
                hprec->m_bFree.store<membar_release>( false ) ;
                return hprec    ;
            }

            // No HP records available for reuse
            // Allocate and push a new HP record
            hprec = NewHPRec()  ;
            hprec->m_idOwner.store<membar_relaxed>( curThreadId )    ;
            hprec->m_bFree.store<membar_relaxed>( false ) ;   // CAS below orders memory access
            hplist_node * pOldHead    ;
            do {
                pOldHead = atomics::load<membar_acquire>( &m_pListHead ) ;
                hprec->m_pNext.store<membar_relaxed>( pOldHead ) ;    // CAS below orders memory access
            } while ( !atomics::cas<membar_acquire>( &m_pListHead, pOldHead, hprec ) ) ;

            return hprec ;
        }

        void GarbageCollector::RetireHPRec( details::HPRec * pRec )
        {
            assert( pRec != NULL )    ;
            CDS_HAZARDPTR_STATISTIC( ++m_Stat.m_RetireHPRec )    ;

            pRec->clear()   ;
            hplist_node * pNode = static_cast<hplist_node *>( pRec )  ;
            //assert( pNode->m_idOwner.value() == cds::OS::getCurrentThreadId() )  ;
            pNode->m_idOwner.store<membar_release>( cds::OS::nullThreadId()) ;
        }

        void GarbageCollector::Scan( details::HPRec * pRec )
        {
            CDS_HAZARDPTR_STATISTIC( ++m_Stat.m_ScanCallCount )    ;

            std::vector< void * >   plist    ;
            plist.reserve( m_nMaxThreadCount * m_nHazardPointerCount )  ;
            assert( plist.size() == 0 ) ;

            // Stage 1: Scan HP list and insert non-null values in plist
            hplist_node * pNode = atomics::load<membar_acquire>( &m_pListHead )  ;

            while ( pNode ) {
                for ( size_t i = 0; i < m_nHazardPointerCount; ++i ) {
                    void * hptr = pNode->m_hzp[i]    ;
                    if ( hptr )
                        plist.push_back( hptr )        ;
                }
                pNode = pNode->m_pNext.load<membar_acquire>()   ;
            }
            assert( plist.size() > 0 )    ;

            // Sort plist to simplify search in
            std::sort( plist.begin(), plist.end() ) ;

            // Stage 2: Search plist
            details::retired_vector& arrRetired = pRec->m_arrRetired    ;

            details::retired_vector::iterator itRetired     = arrRetired.begin()    ;
            details::retired_vector::iterator itRetiredEnd  = arrRetired.end()    ;
            // RetiredPtrVector - собственный класс (не std::vector!), поэтому можем делать clear
            // clear только устанавливает счетчик элементов в 0, не разрушая сам массив
            arrRetired.clear()    ;

            std::vector< void * >::iterator itBegin = plist.begin()    ;
            std::vector< void * >::iterator itEnd = plist.end()    ;
            while ( itRetired != itRetiredEnd ) {
                if ( std::binary_search( itBegin, itEnd, itRetired->m_p) ) {
                    CDS_HAZARDPTR_STATISTIC( ++m_Stat.m_DeferredNode )    ;
                    arrRetired.push( *itRetired )    ;
                }
                else
                    DeletePtr( *itRetired )            ;
                ++itRetired    ;
            }
        }

        void GarbageCollector::HelpScan( details::HPRec * pThis )
        {
            CDS_HAZARDPTR_STATISTIC( ++m_Stat.m_HelpScanCallCount )    ;

            assert( static_cast<hplist_node *>(pThis)->m_idOwner.load<membar_relaxed>() == cds::OS::getCurrentThreadId() ) ;

            const cds::OS::ThreadId nullThreadId = cds::OS::nullThreadId()        ;
            const cds::OS::ThreadId curThreadId = cds::OS::getCurrentThreadId()    ;
            for ( hplist_node * hprec = atomics::load<membar_acquire>( &m_pListHead ); hprec; hprec = hprec->m_pNext.load<membar_acquire>() ) {

                // If m_bFree == true then hprec->m_arrRetired is empty - we don't need to see it
                if ( hprec->m_bFree.load<membar_relaxed>() )
                    continue    ;

                // Owns hprec if it is empty.
                // Several threads may work concurrently so we use atomic technique only.
                const cds::OS::ThreadId curOwner = hprec->m_idOwner.load<membar_acquire >()    ;
                if ( curOwner == nullThreadId || !cds::OS::isThreadAlive( curOwner )) {
                    if ( !hprec->m_idOwner.cas<membar_acq_rel>( curOwner, curThreadId ))
                        continue    ;
                }
                else if ( !hprec->m_idOwner.cas<membar_acq_rel>( nullThreadId, curThreadId ))
                    continue    ;

                // We own the thread successfully. Now, we can see whether HPRec has retired pointers.
                // If it has ones then we move to pThis that is private for current thread.
                details::retired_vector& src = hprec->m_arrRetired    ;
                details::retired_vector& dest = pThis->m_arrRetired   ;
                assert( !dest.isFull())    ;
                details::retired_vector::iterator itRetired = src.begin()    ;
                details::retired_vector::iterator itRetiredEnd = src.end()    ;
                while ( itRetired != itRetiredEnd ) {
                    dest.push( *itRetired ) ;
                    if ( dest.isFull()) {
                        CDS_HAZARDPTR_STATISTIC( ++m_Stat.m_CallScanFromHelpScan )    ;
                        Scan( pThis )   ;
                    }
                    ++itRetired    ;
                }
                src.clear() ;

                hprec->m_bFree.store<membar_relaxed>(true) ;  // next statement orders memory access
                hprec->m_idOwner.store<membar_release>( nullThreadId)    ;
            }
        }

        GarbageCollector::InternalState& GarbageCollector::getInternalState( GarbageCollector::InternalState& stat) const
        {
            stat.nHPCount                = m_nHazardPointerCount    ;
            stat.nMaxThreadCount         = m_nMaxThreadCount        ;
            stat.nMaxRetiredPtrCount     = m_nMaxRetiredPtrCount    ;
            stat.nHPRecSize              = sizeof( hplist_node )
                                            + sizeof(details::retired_ptr) * m_nMaxRetiredPtrCount ;

            stat.nHPRecAllocated         =
                stat.nHPRecUsed              =
                stat.nTotalRetiredPtrCount   =
                stat.nRetiredPtrInFreeHPRecs = 0    ;

            for ( hplist_node * hprec = atomics::load<membar_acquire>( &m_pListHead ); hprec; hprec = hprec->m_pNext.load<membar_acquire>() ) {
                ++stat.nHPRecAllocated  ;
                stat.nTotalRetiredPtrCount += hprec->m_arrRetired.size()    ;

                if ( hprec->m_bFree.load<membar_relaxed>() ) {
                    // Free HP record
                    stat.nRetiredPtrInFreeHPRecs += hprec->m_arrRetired.size()  ;
                }
                else {
                    // Used HP record
                    ++stat.nHPRecUsed   ;
                }
            }

            // Events
            stat.evcAllocHPRec   = m_Stat.m_AllocHPRec  ;
            stat.evcRetireHPRec  = m_Stat.m_RetireHPRec ;
            stat.evcAllocNewHPRec= m_Stat.m_AllocNewHPRec   ;
            stat.evcDeleteHPRec  = m_Stat.m_DeleteHPRec ;

            stat.evcScanCall     = m_Stat.m_ScanCallCount   ;
            stat.evcHelpScanCall = m_Stat.m_HelpScanCallCount   ;
            stat.evcScanFromHelpScan= m_Stat.m_CallScanFromHelpScan ;

            stat.evcDeletedNode  = m_Stat.m_DeletedNode     ;
            stat.evcDeferredNode = m_Stat.m_DeferredNode    ;

            return stat ;
        }


    } //namespace hzp
}} // namespace cds::gc
