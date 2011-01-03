/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_DEFS_H
#define __CDS_DEFS_H

#include <assert.h>
#include <exception>
#include <string>
#include <memory>

#include <boost/static_assert.hpp>

#include <cds/version.h>

/** \mainpage CDS: Concurrent Data Structures library

   This library is a collection of lock-free and lock-based fine-grained algorithms of data structures
   (DS) like maps, queues, list etc. The library contains implementation of well-known data structures
   and memory reclamation schemas for modern processor architectures.

   Supported processor architectures and operating systems (OS) are:
      \li x86 [32bit] linux, windows
      \li amd64 (x86-64) [64bit] linux, windows
      \li ia64 (itanium) [64bit] linux, HP-UX 11.23, HP-UX 11.31
      \li sparc [64bit] sun solaris

   Supported compilers:
      \li GCC 4.3+ - for the UNIX-like OSes
      \li MS Visual C++ 2008 and above - for MS Windows

   For each lock-free DS the CDS library presents several implementation based on published works. For
   example, there are several implementations of queue, each of them is divided by memory reclamation
   schema used. However, any implementation supports common interface of the data structure's type:
    \li map - see \ref map_common_interface
    \li ordered list - see \ref ordered_list_common_interface
    \li queue - see \ref queue_common_interface

   The main part of lock-free DSs is garbage collecting. The garbage collector (GC) solves the problem of safe 
   memory reclamation that is one of the main problems for lock-free programming. 
   The library contains the implementations of several light-weight memory reclamation schemas:
    \li M.Michael's Hazard Pointer - see cds::gc::hzp for more explanation
    \li Gidenstam's memory reclamation schema based on Hazard Pointer and reference counting -
        see cds::gc::hrc
    \li M.Herlihy and M.Moir's Pass The Buck algorithm - see cds::gc::ptb
    \li Tagged pointer (the first memory reclamation schema for the lock-free DS) - see
        cds::gc::tagged

   Many GC requires a support from the thread. The library does not define the threading model you must use,
   it is developed to support various ones; about incorporating CDS library to your threading model see cds::threading.

   The main namespace for the library is \ref cds
*/

/// The main library namespace
namespace cds {}

/** \file
    \brief Basic typedefs and defines

    You do not need include this header directly. All library header files depends on defs.h and include it.

    Defines macros:

    CDS_COMPILER        Compiler:
                    - CDS_COMPILER_MSVC        Microsoft Visual C++
                    - CDS_COMPILER_GCC        GNU C++
                    - CDS_COMPILER_INTEL    Intel C++
                    - CDS_COMPILER_UNKNOWN    unknown compiler

    CDS_COMPILER__NAME    Character compiler name

    CDS_COMPILER_VERSION    Compliler version (number)

    CDS_BUILD_BITS    Resulting binary code:
                    - 32        32bit
                    - 64        64bit
                    - -1        undefined

    CDS_POW2_BITS    CDS_BUILD_BITS == 2**CDS_POW2_BITS

    CDS_PROCESSOR_ARCH    The processor architecture:
                    - CDS_PROCESSOR_X86        Intel x86
                    - CDS_PROCESSOR_AMD64   Amd64, Intel x86-64
                    - CDS_PROCESSOR_IA64    Intel IA64 (Itanium)
                    - CDS_PROCESSOR_SPARC    Sparc
                    - CDS_PROCESSOR_PPC64   PowerPC64
                    - CDS_PROCESSOR_UNKNOWN    undefined processor architecture

    CDS_PROCESSOR__NAME    The name (string) of processor architecture

    CDS_OS_TYPE        Operating system type:
                    - CDS_OS_UNKNOWN            unknown OS
                    - CDS_OS_WIN32            Windows 32bit
                    - CDS_OS_WIN64            Windows 64bit
                    - CDS_OS_LINUX            Linux
                    - CDS_OS_SUN_SOLARIS    Sun Solaris
                    - CDS_OS_HPUX            HP-UX
                    - CDS_OS_CYGWIN            CYGWIN
                    - CDS_OS_AIX            IBM AIX

    CDS_OS__NAME        The name (string) of operating system type

    CDS_OS_INTERFACE OS interface:
                    - CDS_OSI_UNIX             Unix (POSIX)
                    - CDS_OSI_WINDOWS          Windows


    CDS_BUILD_TYPE    Build type: 'RELEASE' or 'DEBUG' string

*/

///@cond
#if defined(_DEBUG) || !defined(NDEBUG)
#    define    CDS_DEBUG
#    define    CDS_BUILD_TYPE    "DEBUG"
#else
#    define    CDS_BUILD_TYPE    "RELEASE"
#endif

// Supported compilers:
#define    CDS_COMPILER_MSVC        1
#define    CDS_COMPILER_GCC        2
#define CDS_COMPILER_INTEL      3
#define    CDS_COMPILER_UNKNOWN    -1

// Supported processor architectures:
#define    CDS_PROCESSOR_X86        1
#define CDS_PROCESSOR_IA64        2
#define CDS_PROCESSOR_SPARC        3
#define CDS_PROCESSOR_AMD64     4
#define CDS_PROCESSOR_PPC64     5   // PowerPC 64bit
#define CDS_PROCESSOR_UNKNOWN    -1

// Supported OS interfaces
#define CDS_OSI_UNKNOWN          0
#define CDS_OSI_UNIX             1
#define CDS_OSI_WINDOWS          2

// Supported operating systems (value of CDS_OS_TYPE):
#define    CDS_OS_UNKNOWN            -1
#define    CDS_OS_WIN32            1
#define    CDS_OS_WIN64            5
#define    CDS_OS_LINUX            10
#define    CDS_OS_SUN_SOLARIS        20
#define    CDS_OS_HPUX                30
#define CDS_OS_CYGWIN            40
#define CDS_OS_AIX              50  // IBM AIX

#if defined(_MSC_VER)
#   if defined(__INTEL_COMPILER)
#       define    CDS_COMPILER            CDS_COMPILER_INTEL
#   else
#       define    CDS_COMPILER            CDS_COMPILER_MSVC
#   endif
#elif defined( __GCC__ ) || defined(__GNUC__)
#    define CDS_COMPILER        CDS_COMPILER_GCC
#else
#    define CDS_COMPILER            CDS_COMPILER_UNKNOWN
#endif  // Compiler choice


// CDS_DEBUG_ASSERT: Debug - assert(_expr); Release - _expr
#ifdef CDS_DEBUG
#    define CDS_DEBUG_ASSERT( _expr )    assert( _expr )
#   define CDS_DEBUG_DO( _expr )        _expr
#else
#    define CDS_DEBUG_ASSERT( _expr )    _expr
#   define CDS_DEBUG_DO( _expr )
#endif

#define CDS_STATIC_ASSERT( _expr )            BOOST_STATIC_ASSERT( _expr )    ;

// Compiler-specific defines
#include <cds/compiler/defs.h>

//@endcond

/*************************************************************************
 Common things
**************************************************************************/

#include <cds/numtraits.h>

namespace cds {

    //@cond
    /// Helper template: converts volatile pointer to non-volatile one
    template <typename T>
    static inline T * non_volatile( T volatile * p ) { return const_cast<T *>( p ); }

    template <typename T>
    static inline T * non_volatile( T * p ) { return p; }
    //@endcond

    /// Base of all exceptions in the library
    class Exception: public std::exception
    {
    protected:
        std::string    m_strMsg    ;    ///< Exception message
    public:
        /// Create empty exception
        Exception()
        {}
        /// Create exception with message
        explicit Exception( const char * pszMsg )
            : m_strMsg( pszMsg )
        {}
        /// Create exception with message
        explicit Exception( const std::string& strMsg )
            :m_strMsg( strMsg )
        {}

        /// Destructor
        virtual ~Exception() throw()
        {}

        /// Return exception message
        virtual const char * what( ) const throw()
        {
            return m_strMsg.c_str()    ;
        }
    };

//@cond
#   define CDS_PURE_VIRTUAL_FUNCTION_CALLED    { assert(false); throw Exception("Pure virtual function called"); }
//@endcond

    /// any_type is used as a placeholder for auto-calculated type (usually in \p rebind templates)
    struct any_type {};

    /** \def CDS_DECLARE_EXCEPTION( _class, _msg )
        Simplifying declaration of specific exception (usual within classes)
        - @p _class - the class name of exception
        - @p _msg - exception message (const char *)
    */
#define CDS_DECLARE_EXCEPTION( _class, _msg )        \
    struct _class: public Exception {                \
    public:                                            \
        _class(): Exception( _msg ) {}                \
    }

} // namespace cds

#endif // #ifndef __CDS_DEFS_H
