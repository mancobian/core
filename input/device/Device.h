///
/// @file Device.h
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

#ifndef RSSD_INPUT_DEVICE_H
#define RSSD_INPUT_DEVICE_H

namespace RSSD {
namespace Input {

class Device
{
public:
  enum Type
  {
    KEYBOARD = 0,
    MOUSE,
    COUNT
  }; /// enum Types

  class State
  {
  public:
    typedef std::tr1::shared_ptr<State> Pointer;

    State(const uint32_t device) : mDeviceType(device) {}
    virtual ~State() {}
    virtual uint32_t getDeviceType() const { return this->mDeviceType; }
    virtual void setDeviceType(const uint32_t value) { assert (value < Device::COUNT); this->mDeviceType = value; }
    virtual bool update(const float64_t elapsed) = 0;

  protected:
    uint32_t mDeviceType;

  private:
    State() {}
  }; /// class State

  class Event
  {
  public:
    typedef std::tr1::shared_ptr<Event> Pointer;

    Event(const uint32_t device, const uint32_t event = 0) :
      mDeviceType(device),
      mEventType(event),
      mTimestamp(0),
      mDeviceState(NULL) {}
    virtual ~Event() {}
    virtual uint32_t getDeviceType() const { return this->mDeviceType; }
    virtual void setDeviceType(const uint32_t value) { assert (value < Device::COUNT); this->mDeviceType = value; }
    virtual uint32_t getEventType() const { return this->mEventType; }
    virtual void setEventType(const uint32_t value) { this->mEventType = value; }
    virtual uint32_t getTimestamp() const { return this->mTimestamp; }
    virtual void setTimestamp(const uint32_t value) { this->mTimestamp = value; }
    virtual State* getDeviceState() const { return this->mDeviceState; }
    virtual void setDeviceState(const State* value) { this->mDeviceState = value; }

  protected:
    uint32_t
      mDeviceType,
      mEventType,
      mTimestamp;
    State::Pointer mDeviceState;

  private:
    Event() {}
  }; /// class Event

  Device(State *state = NULL) : mState(state) {}
  virtual ~Device() { if (this->mState.get()) { this->mState.reset(); } }
  virtual bool update(const float64_t elapsed) = 0;

protected:
  State::Pointer mState;
}; /// struct Device

} /// namespace Input
} /// namespace RSSD

#endif // RSSD_INPUT_DEVICE_H
