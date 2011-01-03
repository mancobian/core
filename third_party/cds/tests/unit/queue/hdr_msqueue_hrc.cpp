/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#include <cds/queue/msqueue_hrc.h>

#include "queue/queue_test_header.h"

namespace queue {
    void Queue_TestHeader::MSQueue_HRC()
    {
        testNoItemCounter<cds::queue::MSQueue<cds::gc::hrc_gc, int> >()    ;
    }

    void Queue_TestHeader::MSQueue_HRC_Counted()
    {
        testWithItemCounter< cds::queue::MSQueue<cds::gc::hrc_gc, int, QueueCountedTraits > >() ;
    }
}
