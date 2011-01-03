/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_THREADING__COMMON_H
#define __CDS_THREADING__COMMON_H

#include <cds/gc/hzp/hzp.h>
#include <cds/gc/hrc/hrc.h>
#include <cds/gc/ptb/ptb.h>

namespace cds {
    /// Threading support
    /**
        The CDS library requires support from the threads.
        Each garbage collector manages a control structure on the per-thread basis.
        The library does not provide any thread model. To embed the library to your application you must provide
        appropriate implementation of cds::threading::Manager interface. This interface manages cds::threading::ThreadData
        structure that contains thread specific data of GCs.
        Any cds::threading::Manager implementation is a singleton and it must be accessible from any thread and from any point of
        your application. Note that you should not mix different implementation of the cds::threading::Manager in single application.

        Before compiling of your application you may define one of CDS_THREADING_xxx macro in cds/user_setup/threading.h:
            \li CDS_THREADING_MSVC - use cds::Threading::Manager implementation based on Microsoft Visual C++ __declspec(thread)
                construction (see cds/threading/msvc.h)
            \li CDS_THREADING_GCC - use cds::Threading::Manager implementation based on GCC __thread
                construction (see cds/threading/gcc.h)
            \li CDS_THREADING_PTHREAD - use cds::Threading::Manager implementation based on pthread thread-specific
                data functions pthread_getspecific/pthread_setspecific

        The abstract cds::threading::Manager interface is:
        \code
        struct Manager {
            // Initializes thread's data. This method must be called in beginning of each thread
            static void attachThread()  ;

            // Frees thread's data. This method must be called in end of the thread that attachThread() was called in
            static void detachThread()  ;

            // Returns gc::hzp::ThreadGC object of current thread
            static gc::hzp_gc::thread_gc&   getHZPGC() ;

            // Returns gc::hrc::ThreadGC object of current thread
            static gc::hrc_gc::thread_gc&   getHRCGC() ;
        }
        \endcode

        The library provides several implementation of cds::threading::Manager in the csd::threading namespace and its children.

        The library's core (dynamic linked library) is free of usage of user-supplied cds::threading::Manager code.
        cds::threading::Manager is necessary for header-only part of CDS library by cds::threading::getGC function call.
    */
    namespace threading {

        /// Thread-specific data
        struct ThreadData {

            char    m_hpManagerPlaceholder[sizeof(gc::hzp::ThreadGC)]   ;   ///< Michael's Hazard Pointer GC placeholder
            char    m_hrcManagerPlaceholder[sizeof(gc::hrc::ThreadGC)]  ;   ///< Gidenstam's GC placeholder
            char    m_ptbManagerPlaceholder[sizeof(gc::ptb::ThreadGC)]  ;   ///< Pass The Buck GC placeholder

            gc::hzp::ThreadGC * m_hpManager     ;   ///< Michael's Hazard Pointer GC thread-specific data
            gc::hrc::ThreadGC * m_hrcManager    ;   ///< Gidenstam's GC thread-specific data
            gc::ptb::ThreadGC * m_ptbManager    ;   ///< Pass The Buck GC thread-specific data

            ThreadData()
            {
                if (gc::hzp::GarbageCollector::isUsed() )
                    m_hpManager = new (m_hpManagerPlaceholder) gc::hzp::ThreadGC ;
                else
                    m_hpManager = NULL  ;

                if ( gc::hrc::GarbageCollector::isUsed() )
                    m_hrcManager = new (m_hrcManagerPlaceholder) gc::hrc::ThreadGC  ;
                else
                    m_hrcManager = NULL ;

                if ( gc::ptb::GarbageCollector::isUsed() )
                    m_ptbManager = new (m_ptbManagerPlaceholder) gc::ptb::ThreadGC  ;
                else
                    m_ptbManager = NULL ;
            }

            ~ThreadData()
            {
                if ( m_hpManager ) {
                    m_hpManager->gc::hzp::ThreadGC::~ThreadGC()     ;
                    m_hpManager = NULL  ;
                }

                if ( m_hrcManager ) {
                    m_hrcManager->gc::hrc::ThreadGC::~ThreadGC()    ;
                    m_hrcManager = NULL ;
                }

                if ( m_ptbManager ) {
                    m_ptbManager->gc::ptb::ThreadGC::~ThreadGC()  ;
                    m_ptbManager = NULL     ;
                }
            }
        };

    } // namespace threading
} // namespace cds::threading

#endif // #ifndef __CDS_THREADING__COMMON_H
