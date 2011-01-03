/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_THREADING_DETAILS_MSVC_H
#define __CDS_THREADING_DETAILS_MSVC_H

#if CDS_COMPILER != CDS_COMPILER_MSVC
#   error "threading/details/msvc.h may be used only with Microsoft Visual C++ compiler"
#endif

#include <cds/threading/details/_common.h>

namespace cds { namespace threading {

    /// cds::threading::Manager implementation based on Microsoft Visual C++ __declspec( thread ) construction
    namespace msvc {

        /// Thread-specific data manager based on MSVC __declspec( thread ) feature
        class Manager {
        private :
            //@cond
            static ThreadData * _threadData()
            {
                typedef unsigned char  ThreadDataPlaceholder[ sizeof(ThreadData) ]  ;
                __declspec( thread ) static ThreadDataPlaceholder threadData        ;

                return reinterpret_cast<ThreadData *>(threadData)   ;
            }
            //@endcond

        public:
            /// This method must be called in beginning of thread execution
            static void attachThread()
            {
                new ( _threadData() ) ThreadData    ;

                if ( gc::hzp::GarbageCollector::isUsed() )
                    _threadData()->m_hpManager->init()   ;
                if ( gc::hrc::GarbageCollector::isUsed() )
                    _threadData()->m_hrcManager->init()  ;
                if ( gc::ptb::GarbageCollector::isUsed() )
                    _threadData()->m_ptbManager->init()  ;
            }

            /// This method must be called in end of thread execution
            static void detachThread()
            {
                if ( gc::ptb::GarbageCollector::isUsed() )
                    _threadData()->m_ptbManager->fini()   ;
                if ( gc::hrc::GarbageCollector::isUsed() )
                    _threadData()->m_hrcManager->fini()  ;
                if ( gc::hzp::GarbageCollector::isUsed() )
                    _threadData()->m_hpManager->fini()   ;

                _threadData()->ThreadData::~ThreadData()   ;
            }

            /// Returns gc::hzp::ThreadGC object of current thread
            /**
                The object returned may be uninitialized if you did not call attachThread in the beginning of thread execution
                or do not use gc::hzp::GarbageCollector.
                To initialize global gc::hzp::GarbageCollector object you must call cds::gc::hzp::GarbageCollector::Construct()
                in the beginning of your application
            */
            static gc::hzp_gc::thread_gc&   getHZPGC()
            {
                assert( _threadData()->m_hpManager != NULL )    ;
                return *(_threadData()->m_hpManager)            ;
            }

            /// Returns gc::hrc::ThreadGC object of current thread
            /**
                The object returned may be uninitialized if you did not call attachThread in the beginning of thread execution
                or do not use gc::hrc::GarbageCollector.
                To initialize global gc::hrc::GarbageCollector object you must call cds::gc::hrc::GarbageCollector::Construct()
                in the beginning of your application
            */
            static gc::hrc_gc::thread_gc&   getHRCGC()
            {
                assert( _threadData()->m_hrcManager != NULL )   ;
                return *(_threadData()->m_hrcManager)           ;
            }

            /// Returns gc::ptb::ThreadGC object of current thread
            /**
                The object returned may be uninitialized if you did not call attachThread in the beginning of thread execution
                or do not use gc::ptb::GarbageCollector.
                To initialize global gc::ptb::GarbageCollector object you must call cds::gc::ptb::GarbageCollector::Construct()
                in the beginning of your application
            */
            static gc::ptb_gc::thread_gc&   getPTBGC()
            {
                assert( _threadData()->m_ptbManager != NULL )   ;
                return *(_threadData()->m_ptbManager)           ;
            }
        } ;

    } // namespace msvc

    using msvc::Manager   ;
} } // namespace cds::threading

#endif // #ifndef __CDS_THREADING_DETAILS_MSVC_H
