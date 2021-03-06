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

#ifndef RSSD_CORE_INPUT_DEVICE_H
#define RSSD_CORE_INPUT_DEVICE_H

#include "System"
#include "Pattern"

namespace RSSD {
namespace Core {
namespace Input {

class Device
{
public:
  typedef Pattern::Factory<Device> Factory;
  typedef Factory::Manager Manager;

  struct Params
  {
    enum Values
    {
      DEVICE_HANDLE = 0,
      WINDOW_HANDLE,
      COUNT
    };
  }; /// enum Params

  struct Types
  {
    enum Values
    {
      UNKNOWN = 0,
      MOUSE,
      KEYBOARD,
      GAMEPAD,
      TOUCH,
      VOICE,
      COUNT
    };
  };

#if 0
  class State
  {
  public:
    typedef std::tr1::shared_ptr<State> Pointer;

    State(const uint_t deviceType) : mDeviceType(deviceType) {}
    virtual ~State() {}
    virtual uint_t getDeviceType() const { return this->mDeviceType; }
    virtual void setDeviceType(const uint_t value) { this->mDeviceType = value; }

  protected:
    uint_t mDeviceType;

  private:
    State() {}
  }; /// class State

  class Event
  {
  public:
    enum Type { UNKNOWN = 0 }; /// enum Type

    Event(const uint_t deviceType, const uint_t eventType = Event::UNKNOWN) : mDeviceType(deviceType), mEventType(eventType)  {}
    virtual ~Event() {}
    virtual uint_t getEventType() const { return this->mEventType; }
    virtual void getEventType(const uint_t value) { this->mEventType = value; }
    virtual uint_t getDeviceType() const { return this->mDeviceType; }
    virtual void getDeviceType(const uint_t value) { this->mDeviceType = value; }

  protected:
    uint_t mDeviceType, mEventType;
  }; /// class Event
#endif

  virtual ~Device() {}
  virtual uint32_t getType() const = 0;
  virtual bool update(const float_t elapsed) = 0;
}; /// class Device

template <typename IMPL>
class BaseDevice : public Device
{
public:
  typedef IMPL ImplType;
  typedef typename IMPL::State StateType;
  typedef typename IMPL::Traits::DeviceHandleType DeviceHandleType;
  typedef typename IMPL::Traits::WindowHandleType WindowHandleType;
  typedef Device::Factory::Impl<BaseDevice<IMPL> > Factory;

  BaseDevice(params_t &params) : mImpl(params) {}
  virtual ~BaseDevice() {}
  virtual uint32_t getType() const { return IMPL::TYPE; }
  virtual bool update(const float_t elapsed) { return this->mImpl.update(elapsed); }

protected:
  ImplType mImpl;
}; /// class BaseDevice

} /// namespace Input
} /// namespace Core
} /// namespace RSSD

#endif // RSSD_CORE_INPUT_DEVICE_H
