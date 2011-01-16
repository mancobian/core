///
/// @file Task.h
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

#ifndef RSSD_CORE_CONCURRENCY_TASK_H
#define RSSD_CORE_CONCURRENCY_TASK_H

#include "System"
#include "Pattern"

namespace RSSD {
namespace Core {
namespace Concurrency {

class Task
{
public:
  typedef Pattern::Factory<Task> Factory;
  typedef Factory::Manager Manager;

  struct Priority
  {
    enum
    {
      UNKNOWN = 0,
      LOW,
      MEDIUM,
      HIGH,
      COUNT
    };
  };

  virtual ~Task() {}
  virtual void run() = 0;
  virtual uint32_t getType() const = 0;
  template <typename TRAITS> static typename TRAITS::IdType generateTaskId()
  {
    return TRAITS::generateTaskId();
  }
}; /// class Task

template <typename TRAITS>
class BaseTask : public Task
{
public:
  typedef SharedPointer<BaseTask<TRAITS> > Pointer;
  typedef typename TRAITS::IdType IdType;
  typedef typename TRAITS::InputType InputType;
  typedef typename TRAITS::OutputType OutputType;
  typedef std::tr1::function<OutputType(InputType)> FunctorType;

  struct Traits
  {
    typedef uint32_t IdType;
    typedef void InputType; /// @note Must be copy-constructible and assignable.
    typedef void OutputType; /// @note Must be copy-constructible and assignable.
    static IdType generateTaskId() { return 0; }
  }; /// struct Traits

  BaseTask(
    const bool recurring = false,
    const uint32_t priority = Task::Priority::MEDIUM,
    const IdType dependency = 0,
    const TRAITS &traits = TRAITS());
  BaseTask(const BaseTask<TRAITS> &rhs);
  virtual ~BaseTask();
  DEFINE_PROPERTY_INLINE(bool, Recurring, mRecurring);
  DEFINE_PROPERTY_INLINE(IdType, Priority, mPriority);
  DEFINE_PROPERTY_INLINE(IdType, TaskId, mTaskId);
  DEFINE_PROPERTY_INLINE(IdType, Dependency, mDependency);
  DEFINE_PROPERTY_INLINE(FunctorType, Functor, mFunctor);
  FORCE_INLINE OutputType operator()(InputType value);

  /// @hack REMOVE THIS!
  virtual void run() {}
  virtual uint32_t getType() const { return 0; }

protected:
  bool mRecurring;
  IdType mPriority;
  IdType mTaskId;
  TRAITS mTraits;
  IdType mDependency;
  FunctorType mFunctor;
}; /// class BaseTask

///
/// Includes
///

#include "concurrency/Task-inl.h"

} /// namespace Concurrency
} /// namespace Core
} /// namespace RSSD

namespace std {

template <>
template <typename TRAITS>
struct less<RSSD::Core::Concurrency::BaseTask<TRAITS> >
{
  bool operator() (
    const typename RSSD::Core::Concurrency::BaseTask<TRAITS>::Pointer &x,
    const typename RSSD::Core::Concurrency::BaseTask<TRAITS>::Pointer &y) const
  {
    if (x->getPriority() == y->getPriority())
      return (x->getTaskID() < y->getTaskId());
    return (x->getPriority() < y->getPriority());
  }
}; /// struct less

} /// namespace std

#endif /// RSSD_CORE_CONCURRENCY_TASK_H
