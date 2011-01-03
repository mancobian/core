///
/// @class Thread
///

class Thread : public BaseThread<boost::thread, boost::thread::native_handle_type>
{
public:
  typedef BaseThread<boost::thread, boost::thread::native_handle_type> Base;
  typedef SharedPointer<Thread> Pointer;

  Thread(const bool start = true) : Base(start) {}
  virtual ~Thread() {}
  boost::thread::native_handle_type getId() const { return this->mThread->native_handle(); }
  virtual void join() { this->mThread->join(); }
  virtual void detach() { this->mThread->detach(); }
  virtual void interrupt() { this->mThread->interrupt(); }
  virtual void yield() { boost::thread::yield(); }
  virtual void sleep(const uint32_t milliseconds) { boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(milliseconds)); }

  static void copy(
    Thread *destination,
    const Thread *source)
  {
    destination = const_cast<Thread*>(source);
  }
}; /// class Thread
