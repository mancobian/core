template <typename FUNCTION>
Task<FUNCTION>::Task() :
  mSet(false)
{
}

template <typename FUNCTION>
Task<FUNCTION>::Task(FUNCTION function) :
  mFunction(function),
  mSet(true)
{
}

template <typename FUNCTION>
Task<FUNCTION>::~Task()
{
}

template <typename FUNCTION>
FORCE_INLINE void Task<FUNCTION>::operator()()
{
  /// Execute task
  if (this->mSet) { this->mFunction(); }

  /// Handle post-execution notification(s)
  this->publish(this);
}

template <typename FUNCTION>
void Task<FUNCTION>::set(FUNCTION function)
{
  this->mSet = true;
  this->mFunction = function;
}
