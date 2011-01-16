///
/// @class Scheduler
///

template <typename TRAITS>
Scheduler<TRAITS>::Scheduler()
{

}

template <typename TRAITS>
Scheduler<TRAITS>::~Scheduler()
{

}

template <typename TRAITS>
bool Scheduler<TRAITS>::registerTask(const TaskType &task)
{
  typename TaskType::Pointer copy(new TaskType(task));
  return this->mImpl.registerTask(copy);
}

template <typename TRAITS>
bool Scheduler<TRAITS>::unregisterTask(const typename TaskType::IdType taskType)
{
  return this->mImpl.unregisterTask(taskType);
}

template <typename TRAITS>
void Scheduler<TRAITS>::clear()
{
  this->mImpl.clear();
}

template <typename TRAITS>
void Scheduler<TRAITS>::schedule()
{
  this->mImpl.schedule();
}

template <typename TRAITS>
void Scheduler<TRAITS>::run(const bool wait)
{
  this->mImpl.run(typename TaskType::InputType(), wait);
}
