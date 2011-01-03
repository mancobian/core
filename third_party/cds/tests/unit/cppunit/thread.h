/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CPPUNIT_THREAD_H
#define __CPPUNIT_THREAD_H

#include "cppunit/cppunit_mini.h"
#include <boost/thread.hpp>
#include <cds/os/timer.h>

namespace CppUnitMini {
    static inline unsigned int Rand( unsigned int nMax )
    {
        double rnd = double( rand() ) / double( RAND_MAX ) ;
        unsigned int n = (unsigned int) (rnd * nMax) ;
        return n < nMax ? n : (n-1)    ;
    }


    class ThreadPool    ;
    class TestThread
    {
    protected:
        typedef TestThread  Base        ;
        friend class ThreadPool         ;

        ThreadPool&         m_Pool      ;
        boost::thread *     m_pThread   ;
        cds::OS::Timer      m_Timer     ;

    public:
        double              m_nDuration ;
        size_t              m_nThreadNo ;

    protected:
        static void threadEntryPoint( TestThread * pThread )    ;

        TestThread( TestThread& src )
            : m_Pool( src.m_Pool )
            , m_pThread( NULL )
            , m_nDuration( 0 )
            , m_nThreadNo( 0 )
        {}

        virtual ~TestThread()
        {
            if ( m_pThread )
                delete m_pThread ;
        }

        virtual TestThread *    clone() = 0 ;

        void create()   ;
        void run()      ;

        virtual void init() {}
        virtual void test() = 0 ;
        virtual void fini() {}

        void error(const char *in_macroName, const char *in_macro, const char *in_file, int in_line) ;

    public:
        TestThread( ThreadPool& pool )
            : m_Pool( pool )
            , m_pThread( NULL )
            , m_nDuration( 0 )
            , m_nThreadNo( 0 )
        {}
    };

    class ThreadPool
    {
    public:
        TestCase&                       m_Test          ;

    private:
        typedef std::vector< TestThread * >     thread_vector   ;

        boost::thread_group             m_Pool          ;
        thread_vector                   m_arrThreads    ;

        boost::barrier * volatile       m_pBarrierStart ;
        boost::barrier * volatile       m_pBarrierDone  ;

    public:
        typedef thread_vector::iterator    iterator    ;

    public:
        ThreadPool( TestCase& tc )
            : m_Test( tc )
            , m_pBarrierStart( NULL )
            , m_pBarrierDone( NULL )
        {}
        ~ThreadPool()   ;

        void    add( TestThread * pThread, size_t nCount )  ;

        void    run()   ;

        void    onThreadInitDone( TestThread * pThread )    ;
        void    onThreadTestDone( TestThread * pThread )    ;
        void    onThreadFiniDone( TestThread * pThread )    ;

        iterator begin() { return m_arrThreads.begin(); }
        iterator end()   { return m_arrThreads.end() ;   }

        double  avgDuration() const
        {
            double nDur = 0 ;
            for ( size_t i = 0; i < m_arrThreads.size(); ++i )
                nDur += m_arrThreads[i]->m_nDuration ;
            return nDur / m_arrThreads.size() ;
        }
    };
}

#endif    // #ifndef __CPPUNIT_THREAD_H
