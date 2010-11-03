///
/// @file Types.h
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

#ifndef RSSD_CORE_SYSTEM_TYPES_H
#define RSSD_CORE_SYSTEM_TYPES_H

#include <cassert>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <string>
#include <utility>
#include <algorithm>
#include <functional>
#include <iterator>
#include <locale>
#include <iostream>
#include <fstream>
#include "ThirdParty.h"

namespace rssd {

///
/// Defines
///

#define USE_EXPORT_KEYWORD 0

///
/// Primitives
///

typedef float float32_t;
typedef double float64_t;

///
/// Pseudo-primitives
///

typedef uint8_t byte;
typedef std::string string_t;
#if defined(RSSD_PLATFORM_POSIX)
typedef std::string string_t;
#elif defined(RSSD_PLATFORM_WINDOWS)
typedef std::wstring string_t;
#endif

///
/// Macros
///

#define TYPEDEF_CONTAINERS( __TYPE ) \
typedef std::vector<__TYPE> __TYPE##_v; \
typedef std::list<__TYPE> __TYPE##_l; \
typedef std::queue<__TYPE> __TYPE##_q; \
typedef std::deque<__TYPE> __TYPE##_d; \
typedef std::set<__TYPE> __TYPE##_s; \
typedef std::map<__TYPE, __TYPE> __TYPE##_m;

#define TYPEDEF_PTR_CONTAINERS( __TYPE ) \
typedef std::vector<__TYPE*> __TYPE##_v; \
typedef std::list<__TYPE*> __TYPE##_l; \
typedef std::queue<__TYPE*> __TYPE##_q; \
typedef std::deque<__TYPE*> __TYPE##_d; \
typedef std::set<__TYPE*> __TYPE##_s; \
typedef std::map<__TYPE*, __TYPE*> __TYPE##_m;

#if USE_EXPORT_KEYWORD
#define EXPORT export
#else
#define EXPORT
#endif

#define DEFINE_PROPERTY(PROPERTY_TYPE, PROPERTY_NAME, CLASS_MEMBER) \
	const PROPERTY_TYPE& get##PROPERTY_NAME() const { return CLASS_MEMBER; } \
	void set##PROPERTY_NAME (const PROPERTY_TYPE &value) { CLASS_MEMBER = value; }

#define DEFINE_INLINE_PROPERTY(PROPERTY_TYPE, PROPERTY_NAME, CLASS_MEMBER) \
	inline const PROPERTY_TYPE& get##PROPERTY_NAME() const { return CLASS_MEMBER; } \
	inline void set##PROPERTY_NAME (const PROPERTY_TYPE &value) { CLASS_MEMBER = value; }

///
/// Typedefs
///

TYPEDEF_CONTAINERS(uint8_t)
TYPEDEF_CONTAINERS(int8_t)
TYPEDEF_CONTAINERS(uint16_t)
TYPEDEF_CONTAINERS(int16_t)
TYPEDEF_CONTAINERS(uint32_t)
TYPEDEF_CONTAINERS(int32_t)
TYPEDEF_CONTAINERS(uint64_t)
TYPEDEF_CONTAINERS(int64_t)
TYPEDEF_CONTAINERS(float32_t)
TYPEDEF_CONTAINERS(float64_t)
TYPEDEF_CONTAINERS(byte)

typedef std::vector<std::string> string_v;
typedef std::list<std::string> string_l;
typedef std::queue<std::string> string_q;
typedef std::deque<std::string> string_d;
typedef std::set<std::string> string_s;
typedef std::map<std::string, std::string> string_m;

typedef boost::variant<
  bool,
  int8_t,
  int16_t,
  int32_t,
  int64_t,
  uint8_t,
  uint16_t,
  uint32_t,
  uint64_t,
  float32_t,
  float64_t,
  std::string> variant_t;

///
/// Aliases
///

// namespace Math = Eigen;

} // namespace rssd

#endif // RSSD_CORE_SYSTEM_TYPES_H

