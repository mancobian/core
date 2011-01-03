/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __UNIT_STD_MAP_GCC_H
#define __UNIT_STD_MAP_GCC_H

#include <map>

#if __GNUC__ * 100 + __GNUC_MINOR__ < 401
#   define CDS_GCC_ALLOCATOR(A)     typename A::template rebind< std::pair<const KEY, VALUE> >::other
#else
#   define CDS_GCC_ALLOCATOR(A)     A
#endif

namespace map {

    template <typename KEY, typename VALUE, typename LOCK, class ALLOCATOR = CDS_DEFAULT_ALLOCATOR>
    class StdMap: public std::map<KEY, VALUE, std::less<KEY>, CDS_GCC_ALLOCATOR(ALLOCATOR)>
    {
        LOCK m_lock    ;
        typedef cds::lock::Auto<LOCK> AutoLock ;
        typedef std::map<KEY, VALUE, std::less<KEY>, CDS_GCC_ALLOCATOR(ALLOCATOR)> base_class ;
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
}   // namespace map

#undef CDS_GCC_ALLOCATOR

#endif  // #ifndef __UNIT_STD_MAP_GCC_H
