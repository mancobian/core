/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#include <cds/queue/lmsqueue_ptb.h>

#include "queue/queue_test_header.h"

namespace queue {

    void Queue_TestHeader::LMSQueue_PTB()
    {
        testNoItemCounter<cds::queue::LMSQueue<cds::gc::ptb_gc, int> >()    ;
    }

    void Queue_TestHeader::LMSQueue_PTB_Counted()
    {
        testWithItemCounter< cds::queue::LMSQueue<cds::gc::ptb_gc, int, QueueCountedTraits > >() ;
    }

}
