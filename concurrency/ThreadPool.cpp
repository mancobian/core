#include "ThreadPool.h"

using namespace RSSD;
using namespace RSSD::Core;
using namespace RSSD::Core::Concurrency;

const uint32_t ThreadPool::DEFAULT_SIZE = RSSD::Core::Concurrency::getHardwareConcurrency();

ThreadPool::ThreadPool(const uint32_t size) :
  mSync(false),
  mRequestedSize(size)
{
  this->resize(size);
}

ThreadPool::~ThreadPool()
{
  this->clear();
}

void ThreadPool::clear()
{
  this->sync();
  this->mIdleThreads.clear();
}

void ThreadPool::resize(const uint32_t size)
{
  this->clear();

  this->mRequestedSize = size;
  if (size == 0) { return; }

  for (uint32_t i = 0; i < size; ++i)
  {
    this->mIdleThreads.push(new Thread());
  }
}

bool ThreadPool::assign(ITask *task)
{
  Thread *thread = NULL;
  if (!this->mIdleThreads.pop(thread))
  {
    this->mPendingTasks.push(task);
    return false;
  }
  this->mActiveThreads.insert(thread->getId(), thread);
  return thread->run();
}

bool ThreadPool::sync()
{
  if (this->mSync) { return false; }
  this->mSync = true;
  this->mBarrier.reset(new Barrier(this->mActiveThreads.size() + 1));
  this->mBarrier->wait();
  this->mSync = false;
  return true;
}

void ThreadPool::onNotification(const IThread *ithread)
{
  /// If sync requested, wait here for remaining active threads to complete
  if (this->mSync) { this->mBarrier->wait(); }

  /// Return thread to idle pool
  Thread *thread = NULL;
  const Thread *constThread = reinterpret_cast<const Thread*>(ithread);
  this->mActiveThreads.find(const_cast<Thread*>(constThread)->getId(), thread, Thread::copy);
  assert (this->mActiveThreads.erase(thread->getId()));
  this->mIdleThreads.push(thread);
}
