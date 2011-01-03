/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#include "cppunit/thread.h"

#include <cds/lock/spinlock.h>

// Multi-threaded stack test for push operation
namespace lock {

#define TEST_CASE( N, L )   void N() { test<L>(); }

    namespace {
        static size_t s_nThreadCount = 8        ;
        static size_t s_nLoopCount = 1000000    ;     // loop count per thread

        static size_t   s_nSharedInt  ;
    }

    class Spinlock_MT: public CppUnitMini::TestCase
    {
        template <class LOCK>
        class Thread: public CppUnitMini::TestThread
        {
            virtual TestThread *    clone()
            {
                return new Thread( *this )  ;
            }
        public:
            LOCK&               m_Lock          ;
            double              m_fTime         ;

        public:
            Thread( CppUnitMini::ThreadPool& pool, LOCK& l )
                : CppUnitMini::TestThread( pool )
                , m_Lock( l )
            {}
            Thread( Thread& src )
                : CppUnitMini::TestThread( src )
                , m_Lock( src.m_Lock )
            {}

            Spinlock_MT&  getTest()
            {
                return reinterpret_cast<Spinlock_MT&>( m_Pool.m_Test )   ;
            }

            virtual void init()
            {}
            virtual void fini()
            {}

            virtual void test()
            {
                m_fTime = m_Timer.duration()    ;

                for ( size_t i  = 0; i < s_nLoopCount; ++i ) {
                    m_Lock.lock()   ;
                    ++s_nSharedInt  ;
                    m_Lock.unlock() ;
                }

                m_fTime = m_Timer.duration() - m_fTime  ;
            }
        };

    protected:
        void setUpParams( const CppUnitMini::TestCfg& cfg ) {
            s_nThreadCount = cfg.getULong("ThreadCount", 8 ) ;
            s_nLoopCount = cfg.getULong("LoopCount", 1000000 );
        }

        template <class LOCK>
        void test()
        {
            LOCK    testLock    ;

            s_nSharedInt = 0  ;

            CppUnitMini::ThreadPool pool( *this )   ;
            pool.add( new Thread<LOCK>( pool, testLock ), s_nThreadCount )  ;

            CPPUNIT_MSG( "   Lock test, thread count=" << s_nThreadCount
                << " loop per thread=" << s_nLoopCount
                << "...")   ;
            cds::OS::Timer      timer   ;
            pool.run()  ;
            CPPUNIT_MSG( "     Duration=" << timer.duration() )     ;

            CPPUNIT_ASSERT_EX( s_nSharedInt == s_nThreadCount * s_nLoopCount,
                "Expected=" << s_nThreadCount * s_nLoopCount
                << " real=" << s_nSharedInt ) ;
        }

        TEST_CASE(spinLock,             cds::lock::Spin             )    ;
        TEST_CASE(spinLock32,           cds::lock::Spin32           )    ;
        TEST_CASE(spinLock64,           cds::lock::Spin64           )    ;
        TEST_CASE(recursiveSpinLock,    cds::lock::ReentrantSpin    )    ;
        TEST_CASE(recursiveSpinLock32,  cds::lock::ReentrantSpin32  )    ;
        TEST_CASE(recursiveSpinLock64,  cds::lock::ReentrantSpin64  )    ;

    protected:
        CPPUNIT_TEST_SUITE(Spinlock_MT)
            CPPUNIT_TEST(spinLock)              ;
            CPPUNIT_TEST(spinLock32)            ;
            CPPUNIT_TEST(spinLock64)            ;
            CPPUNIT_TEST(recursiveSpinLock)     ;
            CPPUNIT_TEST(recursiveSpinLock32)   ;
            CPPUNIT_TEST(recursiveSpinLock64)   ;
        CPPUNIT_TEST_SUITE_END();
    };

} // namespace lock

CPPUNIT_TEST_SUITE_REGISTRATION(lock::Spinlock_MT);
