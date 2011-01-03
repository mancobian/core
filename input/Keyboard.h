///
/// @file KeyboardState.h
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

#ifndef RSSD_CORE_INPUT_KEYBOARD_H
#define RSSD_CORE_INPUT_KEYBOARD_H

#include "input/Device.h"

namespace RSSD {
namespace Core {
namespace Input {

class Keyboard : public BaseDevice<Keyboard>
{
public:
  typedef Device::Factory::Impl<Keyboard> Factory;

  class State : public Device::State
  {
  public:
    State();
    virtual ~State();
    const uint_t getKeyState(const uint_t keycode) const { return this->mKeys[keycode]; }

    static const uint_t NUM_KEYS = 256;

  protected:
    uint_t mKeys[NUM_KEYS];
  }; /// class State

  class Event : public Device::Event
  {
  public:
    enum Type
    {
      UNKNOWN = 0,
      PRESS = 1<<0,
      RELEASE = 1<<1,
      COUNT = 1<<2
    }; /// enum Type

    Event(const uint_t eventType = Event::UNKNOWN);
    virtual ~Event();
  }; /// class Event

  Keyboard();
  virtual ~Keyboard();
}; /// class Keyboard

} /// namespace Input
} /// namespace Core
} /// namespace RSSD

#endif // RSSD_CORE_INPUT_KEYBOARD_H
