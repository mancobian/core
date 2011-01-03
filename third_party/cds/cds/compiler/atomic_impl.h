/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_COMPILER_ATOMIC_IMPL_H
#define __CDS_COMPILER_ATOMIC_IMPL_H

#if CDS_COMPILER == CDS_COMPILER_MSVC
#   if CDS_PROCESSOR_ARCH == CDS_PROCESSOR_X86
#       include <cds/compiler/vc/x86/atomic.h>
#   elif CDS_PROCESSOR_ARCH == CDS_PROCESSOR_AMD64
#       include <cds/compiler/vc/amd64/atomic.h>
#   else
#       error "MS VC++ compiler: unsupported processor architecture"
#   endif
#elif CDS_COMPILER == CDS_COMPILER_GCC
#   if CDS_PROCESSOR_ARCH == CDS_PROCESSOR_X86
#       include <cds/compiler/gcc/x86/atomic.h>
#   elif CDS_PROCESSOR_ARCH == CDS_PROCESSOR_AMD64
#       include <cds/compiler/gcc/amd64/atomic.h>
#   elif CDS_PROCESSOR_ARCH == CDS_PROCESSOR_IA64
#       include <cds/compiler/gcc/ia64/atomic.h>
#   elif CDS_PROCESSOR_ARCH == CDS_PROCESSOR_SPARC
#       include <cds/compiler/gcc/sparc/atomic.h>
#   elif CDS_PROCESSOR_ARCH == CDS_PROCESSOR_PPC64
#       include <cds/compiler/gcc/ppc64/atomic.h>
#   else
#       error "GCC compiler: unsupported processor architecture"
#   endif
#else
#   error "Undefined compiler"
#endif

#endif  // #ifndef __CDS_COMPILER_ATOMIC_IMPL_H
