/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_BACKOFF_STRATEGY_H
#define __CDS_BACKOFF_STRATEGY_H

/*
    Filename: backoff_strategy.h
    Created 2007.03.01 by Maxim.Khiszinsky

    Description:
         Generic back-off strategies

    Editions:
    2007.03.01  Maxim Khiszinsky    Created
    2008.10.02  Maxim Khiszinsky    Backoff action transfers from contructor to operator() for all backoff schemas
    2009.09.10  Maxim Khiszinsky    reset() function added
*/

#include <cds/details/defs.h>
#include <cds/compiler/backoff.h>
#include <cds/os/thread.h>

namespace cds {
    /// Different backoff schemes
    /**
        Back-off schema may be used in lock-free algorithms when the algorithm cannot perform some action because a conflict
        with the other concurrent operation is encountered. In this case current thread can do another work or can call
        processor's performance hint.

        The interface of back-off strategy is following:
        \code
            struct backoff_strategy {
                void operator()()   ;
                void reset() ;
            };
        \endcode

        \p operator() operator calls back-off strategy's action. It is main part of back-off strategy.

        \p reset() function resets internal state of back-off strategy to initial state. It is required for some
        back-off strategies, for example, exponential back-off.
    */
    namespace backoff {

        /// Empty backoff strategy. Do nothing
        struct empty {
            //@cond
            void operator ()() {}
            void reset() {}
            //@endcond
        };

        /// Switch to another thread (yield). Good for thread preemption architecture.
        struct yield {
            //@cond
            void operator ()() { OS::yield(); }
            void reset() {}
            //@endcond
        };

        /// Random pause
        /**
            This back-off strategy
        */
        struct pause {
            //@cond
            void operator ()()
            {
#            ifdef CDS_backoff_pause_defined
                platform::backoff_pause();
#            endif
            }
            void reset() {}
            //@endcond
        };

        /// Default backoff strategy
        typedef yield    Default    ;

        /// Default back-off strategy for lock primitives
        typedef yield   LockDefault ;

    } // namespace backoff
} // namespace cds


#endif // #ifndef __CDS_BACKOFF_STRATEGY_H
