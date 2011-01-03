/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_THREADING_DETAILS_PTHREAD_H
#define __CDS_THREADING_DETAILS_PTHREAD_H

#include <stdio.h>
#include <pthread.h>
#include <cds/threading/details/_common.h>

namespace cds { namespace threading {

    /// cds::threading::Manager implementation based on pthread thread-specific data functions
    namespace pthread {

        /// Thread-specific data manager based on pthread thread-specific data functions
        class Manager {
        private :
            /// pthread error code type
            typedef int pthread_error_code  ;

            /// pthread exception
            class pthread_exception: public cds::Exception {
            public:
                const pthread_error_code    m_errCode   ;   ///< pthread error code
            public:
                /// Exception constructor
                pthread_exception( pthread_error_code nCode, const char * pszFunction )
                    : m_errCode( nCode )
                {
                    char buf[256]   ;
                    sprintf( buf, "Pthread error %i [function %s]", nCode, pszFunction ) ;
                    m_strMsg = buf  ;
                }
            };

            /// pthread TLS key holder
            struct Holder {
            //@cond
                pthread_key_t   m_key;

                Holder(): m_key(0)
                {
                    pthread_error_code  nErr    ;
                    if ( (nErr = pthread_key_create( &m_key, NULL )) != 0 )
                        throw pthread_exception( nErr, "pthread_key_create" )   ;
                }

                ~Holder()
                {
                    pthread_error_code  nErr    ;
                    if ( (nErr = pthread_key_delete( m_key )) != 0 )
                        throw pthread_exception( nErr, "pthread_key_delete" )   ;
                }

                ThreadData *    get()
                {
                    return reinterpret_cast<ThreadData *>( pthread_getspecific( m_key ) )    ;
                }

                void alloc()
                {
                    pthread_error_code  nErr    ;
                    ThreadData * pData = new ThreadData ;
                    if ( ( nErr = pthread_setspecific( m_key, pData )) != 0 )
                        throw pthread_exception( nErr, "pthread_setspecific" )   ;

                    if ( gc::hzp::GarbageCollector::isUsed() )
                        pData->m_hpManager->init()   ;
                    if ( gc::hrc::GarbageCollector::isUsed() )
                        pData->m_hrcManager->init()  ;
                }
                void free()
                {
                    ThreadData * p  ;
                    if ( (p = get()) != NULL ) {
                        if ( gc::hrc::GarbageCollector::isUsed() )
                            p->m_hrcManager->fini()  ;
                        if ( gc::hzp::GarbageCollector::isUsed() )
                            p->m_hpManager->fini()   ;

                        delete p    ;
                    }
#               ifdef _DEBUG
                    pthread_error_code  nErr    ;
                    if ( ( nErr = pthread_setspecific( m_key, NULL )) != 0 )
                        throw pthread_exception( nErr, "pthread_setspecific" )   ;
#               endif
                }
            //@endcond
            };

            //@cond
            enum EThreadAction {
                do_getData,
                do_attachThread,
                do_detachThread,
            };
            //@endcond

            //@cond
            static ThreadData * _threadData( EThreadAction nAction )
            {
                static Holder   holder  ;

                pthread_error_code  nErr    ;
                switch ( nAction ) {
                    case do_getData:
#           ifdef _DEBUG
                        {
                            ThreadData * p = holder.get() ;
                            assert( p != NULL ) ;
                            return p            ;
                        }
#           else
                        return holder.get() ;
#           endif
                    case do_attachThread:
                        holder.alloc()  ;
                        return holder.get() ;
                    case do_detachThread:
                        holder.free()   ;
                        return NULL     ;
                    default:
                        assert( false ) ;   // anything forgotten?..
                }
            }
            //@endcond

        public:
            /// This method must be called in beginning of thread execution
            static void attachThread()
            {
                ThreadData * pData = _threadData( do_attachThread )    ;
                assert( pData != NULL ) ;

                if ( gc::hzp::GarbageCollector::isUsed() )
                    pData->m_hpManager->init()   ;
                if ( gc::hrc::GarbageCollector::isUsed() )
                    pData->m_hrcManager->init()  ;
                if ( gc::ptb::GarbageCollector::isUsed() )
                    pData->m_ptbManager->init()  ;
            }

            /// This method must be called in end of thread execution
            static void detachThread()
            {
                ThreadData * pData = _threadData( do_getData )    ;
                assert( pData != NULL ) ;

                if ( gc::ptb::GarbageCollector::isUsed() )
                    pData->m_ptbManager->fini()  ;
                if ( gc::hrc::GarbageCollector::isUsed() )
                    pData->m_hrcManager->fini()  ;
                if ( gc::hzp::GarbageCollector::isUsed() )
                    pData->m_hpManager->fini()   ;

                _threadData( do_detachThread )    ;
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
                return *(_threadData( do_getData )->m_hpManager)    ;
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
                return *(_threadData( do_getData )->m_hrcManager)   ;
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
                return *(_threadData( do_getData )->m_ptbManager)   ;
            }
        } ;

    } // namespace pthread

    using pthread::Manager   ;
} } // namespace cds::threading

#endif // #ifndef __CDS_THREADING_DETAILS_PTHREAD_H
