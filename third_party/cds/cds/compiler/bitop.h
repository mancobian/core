/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_COMPILER_BITOP_H
#define __CDS_COMPILER_BITOP_H

// Choose appropriate header for current architecture and compiler

#if CDS_COMPILER == CDS_COMPILER_MSVC
/************************************************************************/
/* MS Visual C++                                                        */
/************************************************************************/

#    if CDS_PROCESSOR_ARCH == CDS_PROCESSOR_X86
#        include <cds/compiler/vc/x86/bitop.h>
#   elif CDS_PROCESSOR_ARCH == CDS_PROCESSOR_AMD64
#        include <cds/compiler/vc/amd64/bitop.h>
#    endif

#elif CDS_COMPILER == CDS_COMPILER_GCC
/************************************************************************/
/* GCC                                                                  */
/************************************************************************/

#    if CDS_PROCESSOR_ARCH == CDS_PROCESSOR_X86
#       include <cds/compiler/gcc/x86/bitop.h>
#    elif CDS_PROCESSOR_ARCH == CDS_PROCESSOR_AMD64
#       include <cds/compiler/gcc/amd64/bitop.h>
#    elif CDS_PROCESSOR_ARCH == CDS_PROCESSOR_SPARC
#       include <cds/compiler/gcc/sparc/bitop.h>
#    elif CDS_PROCESSOR_ARCH == CDS_PROCESSOR_IA64
#       include <cds/compiler/gcc/ia64/bitop.h>
#    elif CDS_PROCESSOR_ARCH == CDS_PROCESSOR_PPC64
#       include <cds/compiler/gcc/ppc64/bitop.h>
#    else
#        error "GCC: unknown processor architecture. Compilation is impossible."
#   endif

#else
/************************************************************************/
/* Other compilers                                                      */
/************************************************************************/
#    error Unknown compiler. Compilation aborted.

#endif  // Compiler choice

// Generic (C) implementation
#include <cds/details/bitop_generic.h>

#endif    // #ifndef __CDS_COMPILER_BITOP_H
