/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __UNIT_STD_MAP_VC_H
#define __UNIT_STD_MAP_VC_H

#include <map>

namespace map {
    template <typename KEY, typename VALUE, typename LOCK, class ALLOCATOR = CDS_DEFAULT_ALLOCATOR>
    class StdMap: public std::map<KEY, VALUE, std::less<KEY>, ALLOCATOR>
    {
        LOCK m_lock    ;
        typedef cds::lock::Auto<LOCK> AutoLock ;
        typedef std::map<KEY, VALUE, std::less<KEY>, ALLOCATOR> base_class ;
    public:
        typedef typename base_class::value_type value_type ;

        StdMap( size_t nMapSize, size_t nLoadFactor )
        {}

        bool find( const KEY& key )
        {
            AutoLock al( m_lock )    ;
            return base_class::find( key ) != base_class::end() ;
        }

        bool insert( const KEY& key, const VALUE& val )
        {
            AutoLock al( m_lock )    ;
            return base_class::insert( value_type(key, val)).second ;
        }

        bool erase( const KEY& key )
        {
            AutoLock al( m_lock )   ;
            return base_class::erase( key ) != 0 ;
        }

        std::ostream& dump( std::ostream& stm ) { return stm; }
    };
}

#endif  // #ifndef __UNIT_STD_MAP_VC_H
