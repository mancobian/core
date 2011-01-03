///
/// @file Counter.h
/// @author Mancobian Poemandres
/// @license BSD License
///
/// Copyright (c) MMX by The Secret Design Collective
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
///    * Neither the name of The Secret Design Collective nor the names of its
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

#ifndef RSSD_CORE_CONCURRENCY_COUNTER_H
#define RSSD_CORE_CONCURRENCY_COUNTER_H

#include "System"
#include "concurrency/Atomic.h"

namespace RSSD {
namespace Core {
namespace Concurrency {

class Counter
{
public:
  Counter(const int32_t value = 0) : mValue(value) {}
  ~Counter() {}
  FORCE_INLINE int32_t operator *() const { return this->mValue; }
  FORCE_INLINE int32_t operator ++() { return Atomic::increment(&this->mValue); }
  FORCE_INLINE int32_t operator --() { return Atomic::decrement(&this->mValue); }
  FORCE_INLINE int32_t operator +=(const int32_t value) { return Atomic::add(&this->mValue, value); }
  FORCE_INLINE int32_t operator -=(const int32_t value) { return Atomic::subtract(&this->mValue, value); }

protected:
  int32_t mValue;
}; /// class Counter

} /// namespace Concurrency
} /// namespace Core
} /// namespace RSSD

#endif // RSSD_CORE_CONCURRENCY_COUNTER_H
