/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#include <cds/ordered_list/michael_list_hrc.h>
#include <cds/map/michael_hash_map.h>

#include "map/map_test_header.h"

//#if !defined (STLPORT) || defined(_STLP_USE_NAMESPACES)
//    using namespace std;
//#endif

//
// TestCase class
//
namespace map {

    namespace {
        struct MapTraits: public cds::map::type_traits
        {
            typedef cds::ordered_list::MichaelList<    cds::gc::hrc_gc,
                int,
                int,
                cds::map::pair_traits<int, int>,
                void
            >           bucket_type ;
        };
    }

    void MapTestHeader::MichaelHash_Michael_hrc()
    {
        testWithItemCounter< cds::map::MichaelHashMap<int, int, void, MapTraits> >() ;
    }
}

