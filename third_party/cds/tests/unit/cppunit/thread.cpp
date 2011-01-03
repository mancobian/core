/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#include "cppunit/thread.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace CppUnitMini {

    void TestThread::threadEntryPoint( TestThread * pInst )
    {
        pInst->run()    ;
    }

    void TestThread::create()
    {
        m_pThread = new boost::thread( threadEntryPoint, this ) ;
    }

    void TestThread::run()
    {
        init()  ;
        m_Pool.onThreadInitDone( this )     ;

        test()  ;
        m_Pool.onThreadTestDone( this )     ;

        fini()  ;
        m_Pool.onThreadFiniDone( this )     ;
    }

    void TestThread::error(const char *in_macroName, const char *in_macro, const char *in_file, int in_line)
    {
        m_Pool.m_Test.error( in_macroName, in_macro, in_file, in_line ) ;
    }


    ThreadPool::~ThreadPool()
    {
        if ( m_pBarrierStart )
            delete m_pBarrierStart  ;
        if ( m_pBarrierDone )
            delete m_pBarrierDone   ;

        for ( size_t i = 0; i < m_arrThreads.size(); ++i )
            delete m_arrThreads[i]    ;
        m_arrThreads.resize( 0 )    ;
    }

    void    ThreadPool::add( TestThread * pThread, size_t nCount )
    {
        pThread->m_nThreadNo = m_arrThreads.size()  ;
        m_arrThreads.push_back( pThread )           ;
        while ( --nCount ) {
            TestThread * p = pThread->clone()       ;
            if ( p ) {
                p->m_nThreadNo = m_arrThreads.size();
                m_arrThreads.push_back( p )         ;
            }
        }
    }

    void    ThreadPool::run()
    {
        const size_t nThreadCount = m_arrThreads.size() ;
        m_pBarrierStart = new boost::barrier( (unsigned int) nThreadCount )    ;
        // nThreadCount threads + current thread
        m_pBarrierDone = new boost::barrier( (unsigned int) (nThreadCount + 1) )     ;

        for ( size_t i = 0; i < nThreadCount; ++i )
            m_arrThreads[i]->create()   ;

        // Wait while all threads is done
        m_pBarrierDone->wait()  ;
        boost::posix_time::time_duration sleepTime = boost::posix_time::milliseconds(500)  ;
        boost::this_thread::sleep(sleepTime)  ;
        //::Sleep(500)    ;
    }

    void    ThreadPool::onThreadInitDone( TestThread * pThread )
    {
        // Calls in context of called thread
        // Wait while all threads started
        m_pBarrierStart->wait()     ;

        pThread->m_Timer.reset()    ;
    }

    void    ThreadPool::onThreadTestDone( TestThread * pThread )
    {
        // Calls in context of called thread
        pThread->m_nDuration = pThread->m_Timer.duration()  ;
    }

    void    ThreadPool::onThreadFiniDone( TestThread * pThread )
    {
        // Calls in context of called thread
        // Wait while all threads done
        m_pBarrierDone->wait()  ;
    }
}