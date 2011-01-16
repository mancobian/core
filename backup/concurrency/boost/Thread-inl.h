///
/// @class Thread
///

class Thread :
  public BaseThread<boost::thread, boost::thread::native_handle_type>,
  public RSSD::Core::Pattern::Publisher<const Thread*>
{
public:
  typedef BaseThread<boost::thread, boost::thread::native_handle_type>::Base Base;
  typedef Base::Pointer Pointer;
  typedef Base::Publisher Publisher;
  typedef Base::Subscriber Subscriber;

  Thread(const bool start = true) : Base(start) {}
  virtual ~Thread() {}
  boost::thread::native_handle_type getId() const { return this->mThread->native_handle(); }
  virtual void join() { this->mThread->join(); }
  virtual void detach() { this->mThread->detach(); }
  virtual void interrupt() { this->mThread->interrupt(); }
  virtual void yield() { boost::thread::yield(); }
  virtual void sleep(const uint32_t milliseconds) { boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(milliseconds)); }

  static void copy(
    Thread::Pointer destination,
    const Thread::Pointer source)
  {
    destination = source;
  }
}; /// class Thread
