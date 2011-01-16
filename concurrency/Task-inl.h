///
/// @class BaseTask
///

template <typename TRAITS>
BaseTask<TRAITS>::BaseTask(
  const bool recurring,
  const uint32_t priority,
  const IdType dependency,
  const TRAITS &traits) :
  mRecurring(recurring),
  mPriority(priority),
  mTaskId(Task::generateTaskId<TRAITS>()),
  mTraits(traits),
  mDependency(dependency)
{
  this->setFunctor(std::tr1::bind(&BaseTask<TRAITS>::operator(), *this, _1));
}

template <typename TRAITS>
BaseTask<TRAITS>::BaseTask(const BaseTask<TRAITS> &rhs)
{
  this->mRecurring = rhs.mRecurring;
  this->mPriority = rhs.mPriority;
  this->mTaskId = rhs.mTaskId;
  this->mTraits = rhs.mTraits;
  this->mDependency = rhs.mDependency;
  this->setFunctor(boost::bind(&BaseTask<TRAITS>::operator(), this, _1));
}

template <typename TRAITS>
BaseTask<TRAITS>::~BaseTask()
{
}

template <typename TRAITS>
typename BaseTask<TRAITS>::OutputType BaseTask<TRAITS>::operator()(typename BaseTask<TRAITS>::InputType value)
{
  // this->mInput = value;
  this->run();
  // return this->mOutput;
}
