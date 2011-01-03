FORCE_INLINE void Synchronization::barrier()
{ __sync_synchronize(); }

FORCE_INLINE void Synchronization::sleep(const float milliseconds)
{ ::sleep(milliseconds); }

FORCE_INLINE void Synchronization::switchThread()
{ sleep(0); }

/*
///
/// @class SyncEvent
///

class Event : public IEvent
{
public:
  typedef Synchronization::Factory::Impl<IEvent> Factory;

  virtual ~Event()
  {}

  void init(const bool isManualReset = false, const bool isSignaled = false)
  {
    this->mIsManualReset = isManualReset;
    this->mIsSignaled = isSignaled;
  }

  void trigger()
  {
    Lock lock(this->mMutex);
    this->mIsManualReset
      ? this->mCondition.notify_all()
      : this->mCondition.notify_one();
    this->mIsSignaled = true;
  }

  void reset()
  {
    this->unlock();
  }

  bool wait(const uint32_t milliseconds)
  {
    Lock lock(this->mMutex);
    if (!this->mIsSignaled)
    {
      boost::system_time timeout = boost::get_system_time() + boost::posix_time::milliseconds(milliseconds);
      this->mCondition.timed_wait(lock, timeout);
      if (!this->mIsManualReset) { this->mIsSignaled = false; }
    }
    return !this->mIsSignaled;
  }

  void lock()
  {
    Lock lock(this->mMutex);
    if (!this->mIsSignaled) { this->mCondition.wait(lock); }
  }

  void unlock()
  {
    this->mCondition.notify_all();
    this->mIsSignaled = false;
  }

protected:
  Event() : mIsManualReset(false), mIsSignaled(false)
  {}

  bool mIsManualReset;
  volatile bool mIsSignaled;
  Mutex mMutex;
  Condition mCondition;
}; /// class Event

///
/// @class CriticalSection
///

class CriticalSection : public ICriticalSection
{
public:
  typedef Synchronization::Factory::Impl<ICriticalSection> Factory;

  virtual ~CriticalSection()
  {}

  void lock() { this->mLock.lock(); }

  void unlock() { this->mLock.unlock(); }

protected:
  CriticalSection() : mLock(mMutex, boost::defer_lock)
  {}

  Mutex mMutex;
  Lock mLock;
}; /// class CriticalSection
*/
