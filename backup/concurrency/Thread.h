///
/// @file Thread.h
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
///     this list of conditions and the following disclaimer.
///    * Redistributions in binary form must reproduce the above copyright notice,
///     this list of conditions and the following disclaimer in the documentation
///     and/or other materials provided with the distribution.
///    * Neither the name of The Secret Design Collective nor the names of its
///     contributors may be used to endorse or promote products derived from
///     this software without specific prior written permission.
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

#ifndef RSSD_CORE_CONCURRENCY_THREAD_H
#define RSSD_CORE_CONCURRENCY_THREAD_H

#include "concurrency/Common.h"
#include "concurrency/Task.h"

namespace RSSD {
namespace Core {
namespace Concurrency {

class IThread
{
public:
  virtual ~IThread() {}
  virtual const bool isIdle() const = 0;
  virtual const bool isRunning() const = 0;
  virtual const ITask* getTask() const = 0;
  virtual void setTask(ITask *task) = 0;
  virtual bool start() = 0;
  virtual void stop() = 0;
  virtual bool run() = 0;
  virtual void join() = 0;
  virtual void detach() = 0;
  virtual void interrupt() = 0;
  virtual void yield() = 0;
  virtual void sleep(const uint32_t milliseconds) = 0;
}; /// class IThread

///
/// @class BaseThread
/// @brief This lightweight thread-wrapper exists to provide a
///  simple and consistent interface for various platform-specific
///  thread implementations, e.g. WinThreads, pThreads, Boost threads,
///  etc. Additionally, allocation of thread-local storage for the
///  CDS library is handled RAII-style in the RSSD thread wrapper.
/// @note Threads assume ownership of every Task they are assigned.
///

template <typename THREAD, typename IDTYPE>
class BaseThread :
  public boost::noncopyable,
  public IThread,
  Pattern::Publisher<BaseThread<THREAD, IDTYPE> >
{
public:
  typedef IDTYPE id_t;
  typedef SharedPointer<BaseThread<THREAD, IDTYPE> > Pointer;
  typedef BaseThread<THREAD, IDTYPE> Base;
  typedef Pattern::Publisher<BaseThread<THREAD, IDTYPE> > Publisher;
  typedef typename Base::Publisher::Subscriber Subscriber;

  BaseThread(
    ITask *task,
    const bool start = true) :
    mIdle(true),
    mRunning(false),
    mBarrier(new Barrier(2)),
    mScopedLock(mMutex),
    mTask(task),
    mThread(NULL)
  {
    cds::threading::Manager::attachThread();
    if (start) { this->start(); }
  }

  BaseThread(const bool start = true) :
    mIdle(true),
    mRunning(false),
    mBarrier(new Barrier(2)),
    mScopedLock(mMutex),
    // mTask(NULL)
    mThread(NULL)
  {
    cds::threading::Manager::attachThread();
    if (start) { this->start(); }
  }

  virtual ~BaseThread()
  {
    delete this->mBarrier;
    cds::threading::Manager::detachThread();
  }

  virtual id_t getId() const = 0;

  virtual const bool isIdle() const
  { return this->mIdle; }

  virtual const bool isRunning() const
  { return this->mRunning; }

  virtual const ITask* getTask() const
  { return this->mTask.get(); }

  virtual void setTask(ITask *task)
  { this->mTask.reset(task); }

  virtual bool start()
  {
    if (this->mRunning) { return false; }
    this->mRunning = true;
    this->mThread = new THREAD(&BaseThread<THREAD, IDTYPE>::entryPoint, this);
    return true;
  }

  virtual void stop()
  {
    if (!this->mRunning) { return; }
    this->mRunning = false;
    this->mBarrier->wait();
  }

  virtual bool run()
  {
    if (!this->mRunning || !this->mIdle) { return false; }
    this->mBarrier->wait();
    return true;
  }

protected:
  static void entryPoint(BaseThread<THREAD, IDTYPE> *thread)
  { thread->mainLoop(); }

  virtual void mainLoop()
  {
    this->mBarrier->wait();
    while (this->mRunning)
    {
      /// Run task
      this->mIdle = false;
      (*this->mTask)();

      /// Handle post-execution notification(s)
      this->publish(*this);

      /// Wait on next task
      this->mIdle = true;
      this->mBarrier->wait();
    }
  }

  std::atomic<bool> mIdle, mRunning;
  Barrier *mBarrier;
  boost::mutex mMutex;
  boost::mutex::scoped_lock mScopedLock;
  ITask::Pointer mTask;
  THREAD *mThread;
}; /// class BaseThread

///
/// Includes
///

#if RSSD_BOOST_THREADS || RSSD_GNU_THREADS
#include "concurrency/boost/Thread-inl.h"
#elif RSSD_MICROSOFT_THREADS
#include "concurrency/microsoft/Thread-inl.h"
#endif /// RSSD_*_THREADS

} /// namespace Concurrency
} /// namespace Core
} /// namespace RSSD

#endif /// RSSD_CORE_CONCURRENCY_THREAD_H
