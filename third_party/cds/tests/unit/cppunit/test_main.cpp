/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


/*
 * Copyright (c) 2003, 2004
 * Zdenek Nemec
 *
 * This material is provided "as is", with absolutely no warranty expressed
 * or implied. Any use is at your own risk.
 *
 * Permission to use or copy this software for any purpose is hereby granted
 * without fee, provided the above notices are retained on all copies.
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 *
 */

/*
#if defined(_DEBUG) && _MSC_VER == 1500
#    define _CRTDBG_MAP_ALLOC
#    include <stdlib.h>
#    include <crtdbg.h>
#endif
*/

#include "cppunit/cppunit_proxy.h"
#include "cppunit/file_reporter.h"

#include <cds/threading/model.h>

#include "stdio.h"
#include <fstream>
#include <iostream>
#include <set>

// Visual leak detector (see http://vld.codeplex.com/)
#if defined(CDS_USE_VLD) && CDS_COMPILER == CDS_COMPILER_MSVC
#   ifdef _DEBUG
#       include <vld.h>
#   endif
#endif


std::ostream& operator << (std::ostream& s, const cds::gc::hzp::GarbageCollector::InternalState& stat)
{
    s << "\nHZP GC internal state:"
        << "\n\t\tHP record allocated=" << stat.nHPRecAllocated
        << "\n\t\tHP records used=" << stat.nHPRecUsed
        << "\n\t\tTotal retired ptr count=" << stat.nTotalRetiredPtrCount
        << "\n\t\tRetired ptr in free HP records=" << stat.nRetiredPtrInFreeHPRecs
        << "\n\tEvents:"
        << "\n\t\tHPRec allocations=" << stat.evcAllocHPRec
        << "\n\t\tHPRec retire events=" << stat.evcRetireHPRec
        << "\n\t\tnew HPRec allocations from heap=" << stat.evcAllocNewHPRec
        << "\n\t\tHPRec deletions=" << stat.evcDeleteHPRec
        << "\n\t\tScan calling=" << stat.evcScanCall
        << "\n\t\tHelpScan calling=" << stat.evcHelpScanCall
        << "\n\t\tScan calls from HelpScan=" << stat.evcScanFromHelpScan
        << "\n\t\tretired objects deleting=" << stat.evcDeletedNode
        << "\n\t\tguarded objects on Scan=" << stat.evcDeferredNode
        << std::endl ;

    return s;
}

std::ostream& operator << (std::ostream& s, const cds::gc::hrc::GarbageCollector::internal_state& stat)
{
    s << "\nHRC GC internal state:"
        << "\n\t\tHRC record allocated=" << stat.nHRCRecAllocated
        << "\n\t\tHRC records used=" << stat.nHRCRecUsed
        << "\n\t\tTotal retired ptr count=" << stat.nTotalRetiredPtrCount
        << "\n\t\tRetired ptr in free HRC records=" << stat.nRetiredPtrInFreeHRCRecs
        << "\n\tEvents:"
        << "\n\t\tHRCrec allocations=" << stat.evcAllocHRCRec
        << "\n\t\tHRCrec retire events=" << stat.evcRetireHRCRec
        << "\n\t\tnew HRCrec allocations from heap=" << stat.evcAllocNewHRCRec
        << "\n\t\tHRCrec deletions=" << stat.evcDeleteHRCRec
        << "\n\t\tScan calling=" << stat.evcScanCall
        << "\n\t\tHelpScan calling=" << stat.evcHelpScanCalls
        << "\n\t\tCleanUpAll calling=" << stat.evcCleanUpAllCalls
        << "\n\t\tretired objects deleting=" << stat.evcDeletedNode
        << "\n\t\tguarded nodes on Scan=" << stat.evcScanGuarded
        << "\n\t\tclaimed node on Scan=" << stat.evcScanClaimGuarded
#ifdef _DEBUG
        << "\n\t\tnode constructed count=" << stat.evcNodeConstruct
        << "\n\t\tnode destructed count=" << stat.evcNodeDestruct
#endif
        << std::endl ;

    return s;
}

namespace CppUnitMini
{
  int TestCase::m_numErrors = 0;
  int TestCase::m_numTests = 0;
  std::vector<std::string>  TestCase::m_arrStrings  ;
  bool TestCase::m_bPrintGCState = false            ;
  std::string TestCase::m_strTestDataDir(".")       ;
  Config TestCase::m_Cfg ;

  TestCase *TestCase::m_root = 0;
  Reporter *TestCase::m_reporter = 0;

  void TestCase::registerTestCase(TestCase *in_testCase) {
    in_testCase->m_next = m_root;
    m_root = in_testCase;
  }

  int TestCase::run(Reporter *in_reporter, const char *in_testName, bool invert) {
    TestCase::m_reporter = in_reporter;

    m_numErrors = 0;
    m_numTests = 0;

    TestCase *tmp = m_root;
    while (tmp != 0) {
      tmp->myRun(in_testName, invert);
      tmp = tmp->m_next;
    }

    return m_numErrors;
  }

  void TestCase::print_gc_state()
  {
      if ( m_bPrintGCState ) {
          {
              cds::gc::hzp::GarbageCollector::InternalState stat  ;
              std::cout << cds::gc::hzp::GarbageCollector::instance().getInternalState( stat ) << std::endl ;
          }

          {
              cds::gc::hrc::GarbageCollector::internal_state stat  ;
              std::cout << cds::gc::hrc::GarbageCollector::instance().getInternalState( stat ) << std::endl ;
          }
      }
  }

  void Config::load( const char * fileName )
  {
      std::ifstream s        ;
      s.open( fileName )    ;
      if ( !s.is_open() ) {
          std::cerr << "WARNING: Cannot open test cfg file " << fileName
              << "\n\tUse default settings"
              << std::endl ;
          return    ;
      }

      std::cout << "Using test config file: " << fileName << std::endl ;

      char buf[ 4096 ]    ;

      TestCfg * pMap = NULL ;
      while ( !s.eof() ) {
          s.getline( buf, sizeof(buf)/sizeof(buf[0]) )    ;
          char * pszStr = buf    ;
          // trim left
          while ( *pszStr != 0 && (*pszStr == ' ' || *pszStr == '\t' )) ++pszStr ;
          // trim right
          char * pszEnd = strchr( pszStr, 0 )    ;
          if ( pszEnd == pszStr )    // empty srtring
              continue    ;
          --pszEnd    ;
          while ( pszEnd != pszStr && (*pszEnd ==' ' || *pszEnd=='\t' || *pszEnd=='\n' || *pszEnd=='\r' )) --pszEnd ;

          if ( pszStr == pszEnd  )    // empty string
              continue    ;

          pszEnd[1] = 0    ;

          if ( *pszStr == '#' )    // comment
              continue    ;

          if ( *pszStr == '[' && *pszEnd == ']' ) {    // chapter header
              *pszEnd = 0    ;
              pMap = &( m_Cfg[ pszStr + 1 ] )    ;
              continue    ;
          }

          if ( !pMap )    // ������ - ��� �������� �������
              continue    ;

          char * pszEq = strchr( pszStr, '=' )    ;
          if ( !pszEq )    // ������ ������ ��� ���������, ��� ��������
              continue    ;
          if ( pszEq == pszStr )    // ������ "= ..." - ��� ����� ���������
              continue ;

          pszEnd = pszEq    ;
          while ( pszStr <= --pszEnd && (*pszEnd ==' ' || *pszEnd=='\t' || *pszEnd=='\n' || *pszEnd=='\r') ) ;

          if ( pszEnd <= pszStr )    // ������ ��� ���������
              continue ;
          pszEnd[1] = 0    ;
          pMap->m_Cfg[ pszStr ] = pszEq + 1 ;
      }
      s.close()    ;
  }

  std::vector<std::string> const &    TestCase::getTestStrings()
  {
      if ( m_arrStrings.empty() ) {
          std::string strTestDir = m_strTestDataDir ;

          std::ifstream fDict   ;
          char bufLine[1024]    ;
          std::cout << "Loading test data " << strTestDir << "/dictionary.txt..." << std::endl ;
          fDict.open( (strTestDir + "/dictionary.txt").c_str() )    ;
          if ( fDict.is_open() ) {
              cds::OS::Timer timer ;
              std::string str ;
              fDict >> str    ;   // number of lines in file

              // Assume that dictionary.txt does not contain doubles.
              CppUnitMini::TestCase::m_arrStrings.reserve( atol(str.c_str()) ) ;
              while ( !fDict.eof() ) {
                  fDict.getline( bufLine, sizeof(bufLine)/sizeof(bufLine[0]) )  ;
                  if ( bufLine[0] )
                    m_arrStrings.push_back( bufLine ) ;
              }
              fDict.close()   ;

            /*
              std::set< std::string > SetStr ;
              while ( !fDict.eof() ) {
                  fDict.getline( bufLine, sizeof(bufLine)/sizeof(bufLine[0]) )  ;
                  SetStr.insert( bufLine )    ;
              }
              fDict.close()   ;

              CppUnitMini::TestCase::m_arrStrings.reserve( SetStr.size() ) ;
              for ( std::set<std::string>::const_iterator it = SetStr.begin(); it != SetStr.end(); ++it )
                  CppUnitMini::TestCase::m_arrStrings.push_back( *it )    ;
            */
              std::cout << "  Duration=" << timer.duration() << " String count " << CppUnitMini::TestCase::m_arrStrings.size() << std::endl ;
          }
          else
              std::cout << "  Failed, file not found" << std::endl ;

      }
      return m_arrStrings   ;
  }
}

static void usage(const char* name)
{
  printf("Usage : %s [-t=<class>[::<test>]] [-x=<class>[::<test>]] [-f=<file>] [-m]\n", name);
  printf("\t[-t=<class>[::<test>]] : test class or class::test to execute;\n");
  printf("\t[-x=<class>[::<test>]] : test class or class::test to exclude from execution;\n");
  printf("\t[-d=dir] : test data directory (default is ./data);\n");
  printf("\t[-f=<file>] : output file");
  printf(";\n\t[-m] : monitor test execution, display time duration for each test\n");
  printf("\t[-gc_state] : print gc state after each test\n");
  printf("\t[-cfg=<file>] : config file name for tests\n");
}

int main(int argc, char** argv)
{

#ifdef CDS_MSVC_MEMORY_LEAKS_DETECTING_ENABLED
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

  // CppUnit(mini) test launcher
  // command line option syntax:
  // test [OPTIONS]
  // where OPTIONS are
  //  -t=CLASS[::TEST]    run the test class CLASS or member test CLASS::TEST
  //  -x=CLASS[::TEST]    run all except the test class CLASS or member test CLASS::TEST
  //  -d=dir              test data directory (default is .)
  //  -f=FILE             save output in file FILE instead of stdout
  //  -m                  monitor test(s) execution
  //  -gc_state           print GC state after test
  const char *fileName = 0;
  const char *testName = "";
  const char *xtestName = "";
  const char *testDataDir = "." ;
  const char *cfgFileName =
#ifdef _DEBUG
      "test-debug.conf"
#else
      "test.conf"
#endif
      ;
  bool doMonitoring = false;
  bool printGCstate = false ;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (!strncmp(argv[i], "-t=", 3)) {
        testName = argv[i]+3;
        continue;
      }
      else if (!strncmp(argv[i], "-f=", 3)) {
        fileName = argv[i]+3;
        continue;
      }
      else if (!strncmp(argv[i], "-x=", 3)) {
        xtestName = argv[i]+3;
        continue;
      }
      else if (!strncmp(argv[i], "-d=", 3)) {
          testDataDir = argv[i] + 3;
          continue ;
      }
      else if ( !strncmp(argv[i], "-m", 2)) {
        doMonitoring = true;
        continue;
      }
      else if (!strncmp(argv[i], "-gc_state", 9)) {
          CppUnitMini::TestCase::m_bPrintGCState = true;
          continue ;
      }
      else if( !strncmp(argv[i], "-cfg=", 5)) {
          cfgFileName = argv[i] + 5 ;
          continue ;
      }
    }

    // invalid option, we display normal usage.
    usage(argv[0]);
    return 1;

  }

  CppUnitMini::TestCase::m_strTestDataDir = testDataDir    ;

  CppUnitMini::Reporter* reporter;
  if (fileName != 0)
    reporter = new CppUnitMini::FileReporter(fileName, doMonitoring);
  else
    reporter = new CppUnitMini::FileReporter(stdout, doMonitoring);

  // Load config params
  CppUnitMini::TestCase::m_Cfg.load( cfgFileName ) ;

  // Init CDS runtime
  cds::gc::hzp::GarbageCollector::Construct()    ;
  cds::gc::hrc::GarbageCollector::Construct()    ;
  cds::gc::ptb::GarbageCollector::Construct()   ;

  if ( CppUnitMini::TestCase::m_bPrintGCState ) {
    cds::gc::hzp::GarbageCollector::InternalState stat    ;
    cds::gc::hzp::GarbageCollector::instance().getInternalState( stat )     ;

    std::cout << "HZP GC constants:"
        << "\n\tHP count per thread=" << stat.nHPCount
        << "\n\tMax thread count=" << stat.nMaxThreadCount
        << "\n\tMax retired pointer count per thread=" << stat.nMaxRetiredPtrCount
        << "\n\tHP record size in bytes=" << stat.nHPRecSize
        << "\n" << std::endl ;
  }

  if ( CppUnitMini::TestCase::m_bPrintGCState ) {
      cds::gc::hrc::GarbageCollector::internal_state stat    ;
      cds::gc::hrc::GarbageCollector::instance().getInternalState( stat )     ;

      std::cout << "HRC GC constants:"
          << "\n\tHRC count per thread=" << stat.nHPCount
          << "\n\tMax thread count=" << stat.nMaxThreadCount
          << "\n\tMax retired pointer count per thread=" << stat.nMaxRetiredPtrCount
          << "\n\tHRC record size in bytes=" << stat.nHRCRecSize
          << "\n" << std::endl ;
  }

  // Attach main thread to CDS GC
  cds::threading::Manager::attachThread()   ;

  //// Load test data
  //loadTestData( testDataDir )    ;

  int num_errors;
  if (xtestName[0] != 0) {
    num_errors = CppUnitMini::TestCase::run(reporter, xtestName, true);
  } else {
    num_errors = CppUnitMini::TestCase::run(reporter, testName);
  }

  // Detach main thread from CDS GC
  cds::threading::Manager::detachThread()   ;

  // Finalize CDS runtime
  cds::gc::ptb::GarbageCollector::Destruct()    ;
  cds::gc::hrc::GarbageCollector::Destruct()    ;
  cds::gc::hzp::GarbageCollector::Destruct()    ;

  reporter->printSummary();
  delete reporter;

/*
#ifdef CDS_MSVC_MEMORY_LEAKS_DETECTING_ENABLED
    _CrtDumpMemoryLeaks();
#endif
*/

  return num_errors;
}

// See doc/README.intel for explanation about this code
#if defined (STLPORT) && defined (__ICL) && (__ICL >= 900) && \
            (_STLP_MSVC_LIB < 1300) && defined (_STLP_USE_DYNAMIC_LIB)
#  include <exception>

#  undef std
namespace std
{
  void _STLP_CALL unexpected() {
    unexpected_handler hdl;
    set_unexpected(hdl = set_unexpected((unexpected_handler)0));
    hdl();
  }
}
#endif
