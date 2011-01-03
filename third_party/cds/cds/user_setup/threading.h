/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#ifndef __CDS_USER_SETUP_THREADING_MODEL_H
#define __CDS_USER_SETUP_THREADING_MODEL_H

/**
    CDS threading model

    CDS_THREADING_MSVC - use MS Visual C++ declspec( thread ) declaration to mantain thread-specific data

    CDS_THREADING_GCC - use GCC __thread keyword to mantain thread-specific data

    CDS_THREADING_PTHREAD - use cds::Threading::Manager implementation based on pthread thread-specific
    data functions pthread_getspecific/pthread_setspecific
*/

#if !defined(CDS_THREADING_PTHREAD)
#   if CDS_COMPILER == CDS_COMPILER_MSVC
#        define CDS_THREADING_MSVC
#   elif CDS_COMPILER == CDS_COMPILER_GCC
#       define CDS_THREADING_GCC
#   endif
#endif

#endif    // #ifndef __CDS_USER_SETUP_THREADING_MODEL_H
