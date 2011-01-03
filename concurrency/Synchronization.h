///
/// @file Synchronization.h
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

#ifndef RSSD_CORE_CONCURRENCY_SYNCHRONIZATION_H
#define RSSD_CORE_CONCURRENCY_SYNCHRONIZATION_H

#include "System"
#include "Pattern"

namespace RSSD {
namespace Core {
namespace Concurrency {

///
/// @class Synchronization
///
class Synchronization
{
public:
  typedef Pattern::Factory<Synchronization> Factory;
  typedef Factory::Manager Manager;

  virtual ~Synchronization() {}
  virtual void lock() = 0;
  virtual void unlock() = 0;
  static void barrier();
  static void sleep(const float milliseconds);
  static void switchThread();
}; /// class Synchronization

/*
///
/// @class IEvent
///
class IEvent : public Synchronization
{
public:
  virtual void init(const bool isManualReset = false, const bool isSignaled = false) = 0;
  virtual void trigger() = 0;
  virtual void reset() = 0;
  virtual bool wait(const uint32_t milliseconds = 0xFFFFFFFF) = 0;
}; /// class IEvent

///
/// @class ICriticalSection
///
class ICriticalSection : public Synchronization
{
}; /// class ICriticalSection
*/

#if RSSD_COMPILER_GNU
#include "concurrency/gnu/Synchronization-inl.h"
#elif RSSD_COMPILER_MICROSOFT
#include "concurrency/microsoft/Synchronization-inl.h"
#endif /// RSSD_COMPILER_*

} /// namespace Concurrency
} /// namespace Core
} /// namespace RSSD

#endif /// RSSD_CORE_CONCURRENCY_SYNCHRONIZATION_H
