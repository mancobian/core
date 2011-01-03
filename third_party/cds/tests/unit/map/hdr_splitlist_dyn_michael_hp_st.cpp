/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#include <cds/map/split_ordered_list.h>

#include "map/map_test_header.h"

namespace map {

    namespace {
        struct MapTraits: public cds::map::split_list::type_traits
        {
            typedef cds::ordered_list::michael_list_tag<cds::gc::hzp_gc>   bucket_type ;
        };
    }

    void MapTestHeader::SplitList_Dynamic_Michael_hp()
    {
        testWithItemCounter< cds::map::SplitOrderedList<int, int, void, MapTraits> >() ;
    }
}
