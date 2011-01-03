/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#include <cds/stack/stack_hzp.h>

#include "stack/stack_test_header.h"

namespace stack {
    void StackTestHeader::Stack_hp()
    {
        test< cds::stack::Stack< cds::gc::hzp_gc, int > >() ;
    }
}
