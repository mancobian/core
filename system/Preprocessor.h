///
/// @file Preprocess.h
/// @author Mancobian Poemandres
/// @license BSD License
///
/// Copyright (c) MMX by Royal Society of Secret Design
/// All rights reserved
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
///    * Redistributions of source code must retain the above copyright notice,
/// 		this list of conditions and the following disclaimer.
///    * Redistributions in binary form must reproduce the above copyright notice,
/// 		this list of conditions and the following disclaimer in the documentation
/// 		and/or other materials provided with the distribution.
///    * Neither the name of Royal Society of Secret Design nor the names of its
/// 		contributors may be used to endorse or promote products derived from
/// 		this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
/// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///

#ifndef RSSD_CORE_SYSTEM_PREPROCESSOR_H
#define RSSD_CORE_SYSTEM_PREPROCESSOR_H

namespace RSSD {
namespace Core {

///
/// Utility
///

#define USE_EXPORT_KEYWORD 0
#define FATAL(MESSAGE) __FILE__ (__LINE__): MESSAGE

///
/// Platform
///

// #define RSSD_PLATFORM_LINUX 0
// #define RSSD_PLATFORM_MACINTOSH 0
// #define RSSD_PLATFORM_WINDOWS 0

#if defined(PREDEF_OS_LINUX)
#define RSSD_PLATFORM_LINUX 1
#elif defined(PREDEF_OS_MACINTOSH) || defined(PREDEF_OS_MACOSX)
#define RSSD_PLATFORM_MACINTOSH 1
#elif defined(PREDEF_OS_WINDOWS)
#define RSSD_PLATFORM_WINDOWS 1
#else
#error FATAL("Unrecognized platform.")
#endif

///
/// Compiler (C++)
///

// #define RSSD_COMPILER_BORLAND 0
// #define RSSD_COMPILER_COMEAU 0
// #define RSSD_COMPILER_GNU 0
// #define RSSD_COMPILER_INTEL 0
// #define RSSD_COMPILER_MICROSOFT 0

#if defined(PREDEF_COMPILER_BORLAND)
#define RSSD_COMPILER_BORLAND 1
#elif defined(PREDEF_COMPILER_COMEAU)
#define RSSD_COMPILER_COMEAU 1
#elif defined(PREDEF_COMPILER_GCC)
#define RSSD_COMPILER_GNU 1
#elif defined(PREDEF_COMPILER_INTEL)
#define RSSD_COMPILER_INTEL 1
#elif defined(PREDEF_COMPILER_MICROSOFT)
#define RSSD_COMPILER_MICROSOFT 1
#else
#error FATAL("Unrecognized compiler.")
#endif

///
/// Compiler ABI (C++)
///

#if RSSD_COMPILER_GNU
// #define INLINE __inline__
#define FORCE_INLINE inline __attribute__((always_inline))
#define THREAD_LOCAL __thread
#elif RSSD_COMPILER_MICROSOFT
#define FORCE_INLINE __forceinline
#define THREAD_LOCAL __declspec(thread)
#endif

///
/// Threads
///

#if RSSD_COMPILER_GNU
#define RSSD_BOOST_THREADS 1
#define RSSD_GNU_THREADS 1
#elif RSSD_COMPILER_MICROSOFT
#define RSSD_MICROSOFT_THREADS 1
#endif

}  // namespace Core
}  // namespace RSSD

#endif // RSSD_CORE_SYSTEM_PREPROCESSOR_H
