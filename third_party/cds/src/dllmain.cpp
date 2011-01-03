/*
    This file is a part of libcds - Concurrent Data Structures library
    See http://libcds.sourceforge.net/

    (C) Copyright Maxim Khiszinsky [khizmax at gmail dot com] 2006-2010

    Version 0.7.0
*/


#include <cds/details/defs.h>

#if CDS_OS_TYPE == CDS_OS_WIN32

#include <cds/os/thread.h>

// Visual leak detector (see http://vld.codeplex.com/)
#if defined(CDS_USE_VLD) && CDS_COMPILER == CDS_COMPILER_MSVC
#   ifdef _DEBUG
#       include <vld.h>
#   endif
#endif

static cds::OS::ThreadId    s_MainThreadId = 0    ;
static HINSTANCE            s_DllInstance = NULL;

BOOL WINAPI DllMain(
                HINSTANCE hinstDLL,
                DWORD fdwReason,
                LPVOID /*lpvReserved*/
)
{
    switch ( fdwReason ) {
        case DLL_PROCESS_ATTACH:
            s_DllInstance = hinstDLL    ;
            s_MainThreadId = cds::OS::getCurrentThreadId()    ;
            break    ;
    }
    return TRUE        ;
}

#endif


