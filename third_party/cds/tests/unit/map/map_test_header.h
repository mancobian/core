/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __UNIT_MAP_TEST_HEADER_H
#define __UNIT_MAP_TEST_HEADER_H

#include "cppunit/cppunit_proxy.h"

namespace map {

    namespace {
        static bool bFuncFindCalled = false ;
        static void funcFind( int& n, const int& itemVal )
        {
            bFuncFindCalled = true ;
            n = itemVal ;
        }

        struct FunctorFind {
            void operator ()( int& n, const int& itemVal )
            {
                funcFind( n, itemVal )  ;
            }
        };

        static bool bFuncEmplaceCalled = false ;
        static void funcEmplace( int& nValue, const int& nNewVal )
        {
            bFuncEmplaceCalled = true ;
            nValue = nNewVal    ;
        }

        struct FunctorEmplace {
            void operator ()( int& nValue, const int& nNewVal )
            {
                funcEmplace( nValue, nNewVal ) ;
            }
        };

        static bool bFuncEnsureCalled = false ;
        static void funcEnsure( int& nValue, const int& nNewVal )
        {
            bFuncEnsureCalled = true ;
            nValue = nNewVal ;
        }

        struct FunctorEnsure {
            void operator ()( int& nValue, const int& nNewVal )
            {
                funcEnsure( nValue, nNewVal ) ;
            }
        };
    }

    //
    // Test map operation in single thread mode
    //
    class MapTestHeader : public CppUnitMini::TestCase
    {
    protected:
        static int const s_nItemCount = 100 ;
        //typedef CppUnitMini::TestCase       Base    ;

    protected:
        // Map implementation is valid only with real item counter implementation

        template <class MAP>
        void testWithItemCounter()
        {
            typedef MAP Map ;
            Map     m       ;
            int     nValue  ;
            const int nDefValue = -1    ;

            CPPUNIT_ASSERT( m.empty() ) ;
            CPPUNIT_ASSERT( m.size() == 0 )     ;

            FunctorFind ftorFind    ;
            FunctorEmplace ftorEmplace  ;
            FunctorEnsure ftorEnsure    ;

            std::pair<bool, bool> bEnsure ;

            for ( int i = 1; i <= s_nItemCount; ++i ) {
                CPPUNIT_ASSERT( !m.find(i) )        ;

                nValue = nDefValue  ;
                bFuncFindCalled = false     ;
                CPPUNIT_ASSERT( !m.find(i, nValue, funcFind )) ;
                CPPUNIT_ASSERT( nValue == nDefValue )       ;
                CPPUNIT_ASSERT( !bFuncFindCalled )          ;
                CPPUNIT_ASSERT( !m.find(i, nValue, ftorFind )) ;
                CPPUNIT_ASSERT( nValue == nDefValue )       ;
                CPPUNIT_ASSERT( !bFuncFindCalled )          ;

                // emplace unknown value test
                bFuncEmplaceCalled = false ;
                CPPUNIT_ASSERT( !m.emplace( i, nValue, funcEmplace )) ;
                CPPUNIT_ASSERT( !bFuncEmplaceCalled )       ;
                CPPUNIT_ASSERT( nValue == nDefValue )       ;
                CPPUNIT_ASSERT( !m.emplace( i, nValue, ftorEmplace )) ;
                CPPUNIT_ASSERT( !bFuncEmplaceCalled )       ;
                CPPUNIT_ASSERT( nValue == nDefValue )       ;

                CPPUNIT_ASSERT( m.insert(i, i * s_nItemCount * 10 ) )    ;
                CPPUNIT_ASSERT( m.find(i) )                 ;

                nValue = nDefValue  ;
                bFuncFindCalled = false     ;
                CPPUNIT_ASSERT( m.find(i, nValue, funcFind) )           ;
                CPPUNIT_ASSERT( nValue == i * s_nItemCount * 10 )       ;
                CPPUNIT_ASSERT( bFuncFindCalled )       ;
                bFuncFindCalled = false     ;
                CPPUNIT_ASSERT( m.find(i, nValue, ftorFind) )           ;
                CPPUNIT_ASSERT( nValue == i * s_nItemCount * 10 )       ;
                CPPUNIT_ASSERT( bFuncFindCalled )       ;

                CPPUNIT_ASSERT( !m.insert(i, i * s_nItemCount * 20 ) )   ;
                nValue = nDefValue                          ;
                bFuncFindCalled = false     ;
                CPPUNIT_ASSERT( m.find(i, nValue, funcFind) )         ;
                CPPUNIT_ASSERT( nValue == i * s_nItemCount * 10 )        ;
                CPPUNIT_ASSERT( bFuncFindCalled )       ;
                bFuncFindCalled = false     ;
                CPPUNIT_ASSERT( m.find(i, nValue, ftorFind) )           ;
                CPPUNIT_ASSERT( nValue == i * s_nItemCount * 10 )       ;
                CPPUNIT_ASSERT( bFuncFindCalled )                    ;

                // emplace test
                bFuncEmplaceCalled = false ;
                CPPUNIT_ASSERT( m.emplace( i, i * s_nItemCount * 20, funcEmplace )) ;
                CPPUNIT_ASSERT( bFuncEmplaceCalled )    ;
                bFuncFindCalled = false     ;
                CPPUNIT_ASSERT( m.find(i, nValue, funcFind) )           ;
                CPPUNIT_ASSERT( nValue == i * s_nItemCount * 20 )       ;
                CPPUNIT_ASSERT( bFuncFindCalled )       ;
                bFuncEmplaceCalled = false ;
                CPPUNIT_ASSERT( m.emplace( i, i * s_nItemCount * 10, ftorEmplace )) ;
                CPPUNIT_ASSERT( bFuncEmplaceCalled )    ;
                bFuncFindCalled = false     ;
                CPPUNIT_ASSERT( m.find(i, nValue, funcFind) )           ;
                CPPUNIT_ASSERT( nValue == i * s_nItemCount * 10 )       ;
                CPPUNIT_ASSERT( bFuncFindCalled )       ;

                // ensure test, existing key
                bFuncEnsureCalled = false ;
                bEnsure = m.ensure( i, i * s_nItemCount * 20, funcEnsure )  ;
                CPPUNIT_ASSERT( bEnsure.first )         ;
                CPPUNIT_ASSERT( !bEnsure.second )        ;
                CPPUNIT_ASSERT( bFuncEnsureCalled )     ;
                bFuncFindCalled = false     ;
                CPPUNIT_ASSERT( m.find(i, nValue, funcFind) )           ;
                CPPUNIT_ASSERT( nValue == i * s_nItemCount * 20 )       ;
                CPPUNIT_ASSERT( bFuncFindCalled )       ;
                bFuncEnsureCalled = false ;
                bEnsure = m.ensure( i, i * s_nItemCount * 10, ftorEnsure ) ;
                CPPUNIT_ASSERT( bEnsure.first )         ;
                CPPUNIT_ASSERT( !bEnsure.second )        ;
                CPPUNIT_ASSERT( bFuncEnsureCalled )    ;
                bFuncFindCalled = false     ;
                CPPUNIT_ASSERT( m.find(i, nValue, funcFind) )           ;
                CPPUNIT_ASSERT( nValue == i * s_nItemCount * 10 )       ;
                CPPUNIT_ASSERT( bFuncFindCalled )       ;

                // ensure test, new key
                bFuncEnsureCalled = false ;
                bEnsure = m.ensure( i * s_nItemCount * 10, i * s_nItemCount * 100, funcEnsure ) ;
                CPPUNIT_ASSERT( bEnsure.first )         ;
                CPPUNIT_ASSERT( bEnsure.second )        ;
                CPPUNIT_ASSERT( !bFuncEnsureCalled )    ;
                CPPUNIT_ASSERT( m.find( i * s_nItemCount * 10 ))    ;
                CPPUNIT_ASSERT( m.erase( i * s_nItemCount * 10 ))   ;
                bEnsure = m.ensure( i * s_nItemCount * 10, i * s_nItemCount * 100, ftorEnsure ) ;
                CPPUNIT_ASSERT( bEnsure.first )         ;
                CPPUNIT_ASSERT( bEnsure.second )        ;
                CPPUNIT_ASSERT( !bFuncEnsureCalled )    ;
                CPPUNIT_ASSERT( m.find( i * s_nItemCount * 10 ))    ;
                CPPUNIT_ASSERT( m.erase( i * s_nItemCount * 10 ))   ;

                CPPUNIT_ASSERT( m.size() == (size_t) i )             ;
                CPPUNIT_ASSERT( !m.empty() ) ;
            }

            CPPUNIT_ASSERT( !m.empty() )    ;

            for ( int i = 1; i <= s_nItemCount; ++i ) {
                CPPUNIT_ASSERT( m.find(i) )                 ;
                CPPUNIT_ASSERT( !m.find(i*s_nItemCount*10) )           ;
                nValue = nDefValue                          ;
                bFuncFindCalled = false ;
                CPPUNIT_ASSERT( m.find(i, nValue, funcFind ) )         ;
                CPPUNIT_ASSERT( bFuncFindCalled )  ;
                CPPUNIT_ASSERT( nValue == i * s_nItemCount * 10 )        ;
                nValue = nDefValue                          ;
                bFuncFindCalled = false ;
                CPPUNIT_ASSERT( !m.find(i * s_nItemCount * 10, nValue, ftorFind ) )   ;
                CPPUNIT_ASSERT( !bFuncFindCalled )          ;
                CPPUNIT_ASSERT( nValue == nDefValue )       ;

                CPPUNIT_ASSERT( m.size() == size_t(s_nItemCount - i + 1) )       ;
                CPPUNIT_ASSERT( !m.empty() )            ;

                CPPUNIT_ASSERT( !m.erase(i*s_nItemCount*10))       ;
                CPPUNIT_ASSERT( m.erase(i))             ;

                CPPUNIT_ASSERT( m.size() == size_t(s_nItemCount - i) )       ;

                CPPUNIT_ASSERT( !m.find(i) )                ;
                nValue = nDefValue                          ;
                bFuncFindCalled = false ;
                CPPUNIT_ASSERT( !m.find(i, nValue, funcFind) )        ;
                CPPUNIT_ASSERT( nValue == nDefValue )       ;
                CPPUNIT_ASSERT( !bFuncFindCalled )          ;
            }

            CPPUNIT_ASSERT( m.size() == 0 );
            CPPUNIT_ASSERT( m.empty() )    ;
//            m.clear()   ;
//          CPPUNIT_ASSERT( m.empty() )     ;
        }

    protected:
        void MichaelHash()  ;
        void MichaelHash_Michael_hp()           ;
        void MichaelHash_Michael_hrc()          ;
        void MichaelHash_Michael_ptb()          ;
        void MichaelHash_Michael_tagged()       ;
        void MichaelHash_Lazy_hp()              ;
        void MichaelHash_Lazy_hrc()             ;
        void MichaelHash_Lazy_ptb()             ;

        void SplitList()                        ;
        void SplitList_Static()                 ;
        void SplitList_Dynamic_Michael_hp()     ;
        void SplitList_Static_Michael_hp()      ;
        void SplitList_Dynamic_Michael_hrc()    ;
        void SplitList_Static_Michael_hrc()     ;
        void SplitList_Dynamic_Michael_ptb()    ;
        void SplitList_Static_Michael_ptb()     ;
        void SplitList_Dynamic_Michael_tagged() ;
        void SplitList_Static_Michael_tagged()  ;
        void SplitList_Dynamic_Lazy_hp()        ;
        void SplitList_Static_Lazy_hp()         ;
        void SplitList_Dynamic_Lazy_hrc()       ;
        void SplitList_Static_Lazy_hrc()        ;
        void SplitList_Dynamic_Lazy_ptb()       ;
        void SplitList_Static_Lazy_ptb()        ;

    CPPUNIT_TEST_SUITE(MapTestHeader);
        CPPUNIT_TEST(MichaelHash)                   ;
        CPPUNIT_TEST(MichaelHash_Michael_hp)        ;
        CPPUNIT_TEST(MichaelHash_Michael_hrc)       ;
        CPPUNIT_TEST(MichaelHash_Michael_ptb)       ;
        CPPUNIT_TEST(MichaelHash_Michael_tagged)    ;
        CPPUNIT_TEST(MichaelHash_Lazy_hp)           ;
        CPPUNIT_TEST(MichaelHash_Lazy_hrc)          ;
        CPPUNIT_TEST(MichaelHash_Lazy_ptb)          ;

        CPPUNIT_TEST(SplitList) ;
        CPPUNIT_TEST(SplitList_Static) ;
        CPPUNIT_TEST(SplitList_Dynamic_Michael_hp)      ;
        CPPUNIT_TEST(SplitList_Static_Michael_hp)       ;
        CPPUNIT_TEST(SplitList_Dynamic_Michael_hrc)     ;
        CPPUNIT_TEST(SplitList_Static_Michael_hrc)      ;
        CPPUNIT_TEST(SplitList_Dynamic_Michael_ptb)     ;
        CPPUNIT_TEST(SplitList_Static_Michael_ptb)      ;
        CPPUNIT_TEST(SplitList_Dynamic_Michael_tagged)  ;
        CPPUNIT_TEST(SplitList_Static_Michael_tagged)   ;
        CPPUNIT_TEST(SplitList_Dynamic_Lazy_hp)  ;
        CPPUNIT_TEST(SplitList_Static_Lazy_hp)   ;
        CPPUNIT_TEST(SplitList_Dynamic_Lazy_hrc) ;
        CPPUNIT_TEST(SplitList_Static_Lazy_hrc)  ;
        CPPUNIT_TEST(SplitList_Dynamic_Lazy_ptb) ;
        CPPUNIT_TEST(SplitList_Static_Lazy_ptb)  ;

    CPPUNIT_TEST_SUITE_END();

    };

    class MapTestHeaderNoGC : public CppUnitMini::TestCase
    {
    protected:
        static int const s_nItemCount = 100 ;
        //typedef CppUnitMini::TestCase       Base    ;

    protected:
        // Map implementation is valid only with real item counter implementation
        template <class MAP>
        void testWithItemCounter()
        {
            typedef MAP Map ;
            Map     m       ;
            const int nDefValue = -1    ;
            int * pVal      ;

            FunctorEmplace ftorEmplace  ;
            FunctorEnsure ftorEnsure    ;

            std::pair<bool, bool> pairRet ;

            CPPUNIT_ASSERT( m.empty() ) ;
            CPPUNIT_ASSERT( m.size() == 0 )     ;

            for ( int i = 1; i <= s_nItemCount; ++i ) {
                CPPUNIT_ASSERT( !m.find(i) )        ;
                CPPUNIT_ASSERT( m.get(i) == NULL ) ;
                CPPUNIT_ASSERT( m.insert(i, i * s_nItemCount * 10 ) )    ;
                CPPUNIT_ASSERT( m.find(i) )                 ;
                pVal = m.get(i) ;
                CPPUNIT_ASSERT( pVal != NULL )              ;
                if ( pVal != NULL ) {
                    CPPUNIT_ASSERT( *pVal == i * s_nItemCount * 10 )    ;
                }

                CPPUNIT_ASSERT( !m.insert(i, i * s_nItemCount * 20 ) )   ;
                pVal = m.get(i) ;
                CPPUNIT_ASSERT( pVal != NULL )              ;
                if ( pVal != NULL ) {
                    CPPUNIT_ASSERT( *pVal == i * s_nItemCount * 10 )    ;
                }

                // ensure test
                bFuncEnsureCalled = false ;
                pairRet = m.ensure(i, i * s_nItemCount * 20, funcEnsure ) ;
                CPPUNIT_ASSERT( pairRet.first )     ;
                CPPUNIT_ASSERT( !pairRet.second )   ;
                CPPUNIT_ASSERT( bFuncEnsureCalled ) ;
                pVal = m.get(i) ;
                CPPUNIT_ASSERT( pVal != NULL )              ;
                if ( pVal != NULL ) {
                    CPPUNIT_ASSERT( *pVal == i * s_nItemCount * 20 )    ;
                }
                bFuncEnsureCalled = false ;
                pairRet = m.ensure(i, i * s_nItemCount * 10, ftorEnsure ) ;
                CPPUNIT_ASSERT( pairRet.first )     ;
                CPPUNIT_ASSERT( !pairRet.second )   ;
                CPPUNIT_ASSERT( bFuncEnsureCalled ) ;
                pVal = m.get(i) ;
                CPPUNIT_ASSERT( pVal != NULL )              ;
                if ( pVal != NULL ) {
                    CPPUNIT_ASSERT( *pVal == i * s_nItemCount * 10 )    ;
                }

                // emplace test
                bFuncEmplaceCalled = false ;
                CPPUNIT_ASSERT( m.emplace( i, i * s_nItemCount * 20, funcEmplace )) ;
                CPPUNIT_ASSERT( bFuncEmplaceCalled )    ;
                bFuncFindCalled = false     ;
                pVal = m.get(i) ;
                CPPUNIT_ASSERT( pVal != NULL )              ;
                if ( pVal != NULL ) {
                    CPPUNIT_ASSERT( *pVal == i * s_nItemCount * 20 )    ;
                }
                bFuncEmplaceCalled = false ;
                CPPUNIT_ASSERT( m.emplace( i, i * s_nItemCount * 10, ftorEmplace )) ;
                CPPUNIT_ASSERT( bFuncEmplaceCalled )    ;
                bFuncFindCalled = false     ;
                pVal = m.get(i) ;
                CPPUNIT_ASSERT( pVal != NULL )              ;
                if ( pVal != NULL ) {
                    CPPUNIT_ASSERT( *pVal == i * s_nItemCount * 10 )    ;
                }

                CPPUNIT_ASSERT( m.size() == (size_t) i )             ;
                CPPUNIT_ASSERT( !m.empty() ) ;
            }

            CPPUNIT_ASSERT( !m.empty() )    ;
            //m.clear()   ;
            //CPPUNIT_ASSERT( m.empty() )    ;
        }

    protected:
        void MichaelHash_Michael()           ;
        void MichaelHash_Lazy()              ;

        void SplitList_Dynamic_Michael()     ;
        void SplitList_Static_Michael()      ;
        void SplitList_Dynamic_Lazy()        ;
        void SplitList_Static_Lazy()         ;


        CPPUNIT_TEST_SUITE(MapTestHeader);
            CPPUNIT_TEST(MichaelHash_Michael)        ;
            CPPUNIT_TEST(MichaelHash_Lazy)           ;

            CPPUNIT_TEST(SplitList_Dynamic_Michael)  ;
            CPPUNIT_TEST(SplitList_Static_Michael)   ;
            CPPUNIT_TEST(SplitList_Dynamic_Lazy)  ;
            CPPUNIT_TEST(SplitList_Static_Lazy)   ;
        CPPUNIT_TEST_SUITE_END();

    };

} // namespace map

#endif // #ifndef __UNIT_MAP_TEST_HEADER_H
