///
/// @file ThreadPool.h
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

#ifndef RSSD_CORE_CONCURRENCY_THREADPOOL_H
#define RSSD_CORE_CONCURRENCY_THREADPOOL_H

#include "System"
#include "concurrency/Common.h"
#include "concurrency/Thread.h"

namespace RSSD {
namespace Core {
namespace Concurrency {
class ITask;
}
}
}

namespace RSSD {
namespace Core {
namespace Concurrency {

class ThreadPool : public Thread::Subscriber
{
public:
  typedef SharedPointer<Barrier> BarrierPointer;

  ThreadPool(const uint32_t size = DEFAULT_SIZE);
  virtual ~ThreadPool();
  FORCE_INLINE BarrierPointer getBarrier() { return this->mBarrier; }
  FORCE_INLINE uint32_t numIdleThreads() const { return this->mIdleThreads.size(); }
  FORCE_INLINE uint32_t numActiveThreads() const { return this->mActiveThreads.size(); }
  FORCE_INLINE uint32_t numTotalThreads() const { return (this->mIdleThreads.size() + this->mActiveThreads.size()); }
  virtual void resize(const uint32_t size);
  virtual void clear();
  virtual bool sync();
  virtual bool assign(ITask *task);

protected:
  virtual void onNotification(const IThread *thread);

  static const uint32_t DEFAULT_SIZE;
  volatile bool mSync;
  volatile uint32_t mRequestedSize;
  BarrierPointer mBarrier;
  Containers<Thread*>::Queue mIdleThreads;
  Containers<ITask*>::Queue mPendingTasks;
  Containers<Thread*>::Associative<Thread::id_t>::Map mActiveThreads;
}; /// class ThreadPool

} /// namespace Concurrency
} /// namespace Core
} /// namespace RSSD

#endif /// RSSD_CORE_CONCURRENCY_THREADPOOL_H
