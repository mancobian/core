///
/// @file Atomic.h
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

#ifndef RSSD_CORE_CONCURRENCY_ATOMIC_H
#define RSSD_CORE_CONCURRENCY_ATOMIC_H

#include "System"
#include

namespace RSSD {
namespace Core {
namespace Concurrency {
namespace Atomic {

int32_t increment(int32_t volatile *addend);
int32_t decrement(int32_t volatile *addend);
/// @note If [destination] == [comperand] then
///  assign the value of [swap] to [destination]
///  and return the previous value of [destination].
int32_t compareAndSwap(int32_t volatile *destination, int32_t swap, int32_t comperand);
int64_t compareAndSwap(int64_t volatile *destination, int64_t swap, int64_t comperand);
int32_t swap(int32_t volatile *target, int32_t value);
int32_t swapAndAdd(int32_t volatile *addend, int32_t value);
int32_t add(int32_t volatile *addend);
int32_t subtract(int32_t volatile *addend);

#if RSSD_COMPILER_GNU
#include "concurrency/gnu/Atomic-inl.h"
#elif RSSD_COMPILER_MICROSOFT
#include "concurrency/microsoft/Atomic-inl.h"
#endif /// RSSD_COMPILER_*

} /// namespace Atomic
} /// namespace Concurrency
} /// namespace Core
} /// namespace RSSD

#endif /// RSSD_CORE_CONCURRENCY_ATOMIC_H
