/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_QUEUE_LMSQUEUE_H
#define __CDS_QUEUE_LMSQUEUE_H

/*
    Editions:
        2009.09.09 Maxim Khiszinsky Created
*/

#include <cds/queue/details/queue_base.h>
#include <cds/details/allocator.h>

namespace cds {
    namespace queue {


        /** \class LMSQueue
            \brief Ladan-Mozes & Shavit optimistic queue

            Ladan-Mozes & Shavit optimistic queue algorithm.
            \par Template parameters
                \li \p GC    lock-free reclamation schema (garbage collector)
                \li \p T    type of item stored in queue
                \li \p TRAITS class traits. The default is traits
                \li \p ALLOCATOR node allocator. The default is CDS_DEFAULT_ALLOCATOR.

            There are specialization for each appropriate reclamation schema \p GC.

            Source: [2004] Edya Ladan-Mozes, Nir Shavit "An Optimistic Approach to Lock-Free FIFO Queues"
        */
        template <
            typename GC,
            typename T,
            typename TRAITS = traits,
            class ALLOCATOR = CDS_DEFAULT_ALLOCATOR
        >
        class LMSQueue    ;

    }   // namespace queue
}   // namespace cds

#endif // #ifndef __CDS_QUEUE_LMSQUEUE_H
