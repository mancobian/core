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
/// 		this list of conditions and the following disclaimer.
///    * Redistributions in binary form must reproduce the above copyright notice,
/// 		this list of conditions and the following disclaimer in the documentation
/// 		and/or other materials provided with the distribution.
///    * Neither the name of The Secret Design Collective nor the names of its
/// 		contributors may be used to endorse or promote products derived from
/// 		this software without specific prior written permission.
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

class ITask
{
public:
  typedef SharedPointer<ITask> Pointer;

  virtual ~ITask() {}
  virtual void operator()() = 0;
}; /// class ITask

template <typename FUNCTION>
class Task :
  boost::noncopyable,
  public ITask,
  public RSSD::Core::Pattern::Publisher<Task<FUNCTION> >
{
public:
  typedef Pattern::Publisher<Task<FUNCTION> > Publisher;
  typedef typename Publisher::Subscriber Subscriber;
  typedef SharedPointer<Task> Pointer;

  Task();
  Task(FUNCTION function);
  virtual ~Task();
  virtual void operator()();
  void set(FUNCTION function);

protected:
  bool mSet;
  FUNCTION mFunction;
}; /// class Task

///
/// Includes
///

#include "concurrency/Task-inl.h"

} /// namespace Concurrency
} /// namespace Core
} /// namespace RSSD

#endif /// RSSD_CORE_CONCURRENCY_TASK_H
