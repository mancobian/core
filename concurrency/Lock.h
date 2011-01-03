///
/// @file Lock.h
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

#ifndef RSSD_CORE_CONCURRENCY_LOCK_H
#define RSSD_CORE_CONCURRENCY_LOCK_H

#include "System"

namespace RSSD {
namespace Core {
namespace Concurrency {

// SpinLocks are efficient if threads are only likely to be blocked for a short period of time, as they avoid overhead from operating
// system process re-scheduling or context switching. However, spinlocks become wasteful if held for longer durations, both preventing
// other threads from running and requiring re-scheduling. The longer a lock is held by a thread, the greater the risk that it will be
// interrupted by the O/S scheduler while holding the lock. If this happens, other threads will be left "spinning" (repeatedly trying
// to acquire the lock), while the thread holding the lock is not making progress towards releasing it. The result is a semi-deadlock
// until the thread holding the lock can finish and release it. This is especially true on a single-processor system, where each waiting
// thread of the same priority is likely to waste its quantum (allocated time where a thread can run) spinning until the thread that
// holds the lock is finally finished.
template <bool WAIT, bool SLEEP>
class SpinLock
{
public:
  FORCE_INLINE SpinLock() : mSemaphore(0)
  {}

  ~SpinLock()
  {}

  FORCE_INLINE bool lock()
  {
    do
    {
      /// Atomically swap the lock variable with 1 if it's currently equal to 0
      if (this->mSemaphore.compare_exchange_weak(0, 1))
      {
        // We successfully acquired the lock
        Synchronization::barrier();
        return true;
      }

      // To reduce inter-CPU bus traffic, when the lock is not acquired, loop reading without trying to write anything, until
      // the value changes. This optimization should be effective on all CPU architectures that have a cache per CPU.
      while (WAIT && this->mSemaphore)
      {
        if (SLEEP) { Synchronization::switchThread(); }
      }
    } while (WAIT);
    return false;
  }

  FORCE_INLINE void unlock()
  {
    Synchronization::barrier();
    this->mSemaphore = 0;
  }

protected:
    volatile std::atomic_int mSemaphore;
}; /// class SpinLock

} /// namespace Concurrency
} /// namespace Core
} /// namespace RSSD

#endif /// RSSD_CORE_CONCURRENCY_LOCK_H
