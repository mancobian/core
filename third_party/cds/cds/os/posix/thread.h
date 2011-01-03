/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_OS_POSIX_THREAD_H
#define __CDS_OS_POSIX_THREAD_H

#include <stdlib.h>     // system
#include <pthread.h>
#include <sched.h>
#include <cerrno>
#include <cstdlib>
#include <string>
#include <string.h>

namespace cds { namespace OS {
    namespace posix {
        typedef pthread_t       ThreadId    ;

        CDS_CONSTEXPR static inline ThreadId nullThreadId()   { return 0 ; }
        static inline ThreadId getCurrentThreadId()    { return pthread_self() ; }

        static inline bool isThreadAlive( ThreadId id )
        {
            int shPolicy    ;
            sched_param shParam ;
            return pthread_getschedparam( id, &shPolicy, &shParam ) == 0 ;
        }

        static inline void yield()      { sched_yield(); }
        static inline void backoff()    { sched_yield(); }

    }    // namespace posix

    using posix::ThreadId        ;
    using posix::ThreadId        ;

    using posix::nullThreadId    ;
    using posix::getCurrentThreadId    ;
    using posix::isThreadAlive    ;
    using posix::yield            ;
    using posix::backoff        ;

}} // namespace cds::OS


#endif // #ifndef __CDS_OS_POSIX_THREAD_H
