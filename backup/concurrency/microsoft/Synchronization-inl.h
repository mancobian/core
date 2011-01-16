///
/// @class Synchronization
///

FORCE_INLINE void Synchronization::barrier()
{ ::MemoryBarrier(); }

FORCE_INLINE void Synchronization::sleep(const float milliseconds)
{ ::Sleep(milliseconds); }

FORCE_INLINE void Synchronization::switchThread()
{ sleep(0); }

/*
///
/// @class SyncEvent
///

class SyncEvent : public ISyncEvent
{
public:
  typedef ISyncBase::Factory::Impl<ISyncEvent> Factory;

  virtual ~SyncEvent()
  { CloseHandle(this->mEventHandle); }

  void init(const bool isManualReset = false, const bool isSignaled = false)
  {
    if (this->mEventHandle) { CloseHandle(this->mEventHandle); }
    this->mEventHandle = CreateEventA(NULL, isManualReset, isSignaled, 0);
  }

  void trigger()
  { SetEvent(this->mEventHandle); }

  void reset()
  { ResetEvent(this->mEventHandle); }

  bool wait(const uint32_t milliseconds)
  { return WaitForSingleObject(this->mEventHandle, milliseconds) == WAIT_OBJECT_0; }

  void lock()
  { WaitForSingleObject(this->mEventHandle, INFINITE); }

  void unlock()
  { PulseEvent(this->mEventHandle); }

protected:
  SyncEvent() : mEventHandle(NULL)
  {}

  HANDLE mEventHandle;
}; /// class SyncEvent

///
/// @class CriticalSection
///

class CriticalSection : public ICriticalSection
{
public:
  typedef ISyncBase::Factory::Impl<ICriticalSection> Factory;

  virtual ~CriticalSection()
  { DeleteCriticalSection(&this->mCriticalSection); }

  void lock()
  { EnterCriticalSection(&this->mCriticalSection); }

  void unlock()
  { LeaveCriticalSection(&this->mCriticalSection); }

protected:
  CriticalSection()
  { InitializeCriticalSection(&this->mCriticalSection); }

  CRITICAL_SECTION mCriticalSection;
}; /// class CriticalSection
*/
