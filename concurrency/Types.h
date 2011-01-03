///
/// @file Types.h
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

#ifndef RSSD_CORE_CONCURRENCY_TYPES_H
#define RSSD_CORE_CONCURRENCY_TYPES_H

#include "System"

namespace RSSD {
namespace Core {
namespace Concurrency {

typedef std::mutex Mutex;
typedef boost::barrier Barrier;
typedef std::condition_variable Condition;
typedef boost::unique_lock<Mutex> ScopedLock;
typedef cds::SpinLock SpinLock;

template <typename T>
struct Containers
{
  typedef cds::gc::hzp_gc GarbageCollector;
  struct CountedTraits : public cds::queue::traits
  {
    typedef cds::atomics::item_counter<cds::membar_release, cds::membar_acquire> counter_t;
  };

  typedef cds::queue::LMSQueue<GarbageCollector, T> Queue;
  typedef cds::queue::LMSQueue<GarbageCollector, T, CountedTraits> QueueCounted;

  template <typename KEY>
  struct Associative
  {
    typedef cds::map::SplitOrderedList<KEY, T> Map;
  }; /// struct Containers
}; /// struct Containers

} /// namespace Concurrency
} /// namespace Core
} /// namespace RSSD

#endif // RSSD_CORE_CONCURRENCY_TYPES_H
