/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_COMPILER_GCC_SPARC_BACKOFF_H
#define __CDS_COMPILER_GCC_SPARC_BACKOFF_H

//@cond none

namespace cds { namespace backoff {
    namespace gcc { namespace Sparc {

#        define CDS_backoff_pause_defined
        static inline void backoff_pause( unsigned int nLoop = 0x000003FF )
        {

        }

    }} // namespace gcc::Sparc

    namespace platform {
        using namespace gcc::Sparc ;
    }
}}  // namespace cds::backoff

//@endcond
#endif  // #ifndef __CDS_COMPILER_GCC_SPARC_BACKOFF_H
