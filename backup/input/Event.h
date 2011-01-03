///
/// @file Event.h
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

#ifndef NOUS_INPUT_EVENT_H
#define NOUS_INPUT_EVENT_H

#include <core/Core.h>
#include <input/Device.h>
#include <input/Event.h>
#include <network/Network.h>
#include <event/Event.h>

namespace Nous {
namespace Input {

class DeviceEvent : public Event::Packet
{
public:
	DeviceEvent(const Device::Type type) :
		Event::Packet(Event::Input),
		_device(type),
		_device_id(0)
	{}

	virtual ~DeviceEvent()
	{}

public:
	inline Device::Type getDevice() const { return this->_device; }
	inline void setDevice(const Device::Type value) { this->_device = value; }
	inline uint32_t getDeviceId() const { return this->_device_id; }
	inline void setDeviceId(const uint32_t value) { this->_device_id = value; }
	inline virtual uint32_t getSize() const
	{
		return Event::Packet::getSize()
			+ sizeof(this->_device)
			+ sizeof(this->_device_id);
	}

public:
	virtual byte_v serialize() const
	{
		byte_v data = Event::Packet::serialize();
		data.insert(data.end(),
			((char*)&this->_device)[0],
			((char*)&this->_device)[sizeof(this->_device)]);
		data.insert(data.end(),
			((char*)&this->_device_id)[0],
			((char*)&this->_device_id)[sizeof(this->_device_id)]);
		return data;
	}

protected:
	Device::Type _device;
	uint32_t _device_id;
}; // class DeviceEvent

class KeyboardEvent : public DeviceEvent
{
public:
	KeyboardEvent() :
		DeviceEvent(Device::KEYBOARD),
		_event(Device::Keyboard::UNKNOWN),
		_keycode(0)
	{}

	virtual ~KeyboardEvent()
	{}

public:
	inline virtual uint32_t getSize() const
	{
		return DeviceEvent::getSize()
			+ sizeof(this->_event)
			+ sizeof(this->_keycode);
	}

public:
	inline Device::Keyboard::Event getEvent() const { return this->_event; }
	inline void setEvent(const Device::Keyboard::Event value) { this->_event = value; }
	inline uint32_t getKeycode() const { return this->_keycode; }
	inline void setKeycode(const uint32_t value) { this->_keycode = value; }

public:
	virtual byte_v serialize() const
	{
		byte_v data = DeviceEvent::serialize();
		data.insert(data.end(),
			((char*)&this->_event)[0],
			((char*)&this->_event)[sizeof(this->_event)]);
		data.insert(data.end(),
			((char*)&this->_keycode)[0],
			((char*)&this->_keycode)[sizeof(this->_keycode)]);
		return data;
	}

protected:
	Device::Keyboard::Event _event;
	uint32_t _keycode;
}; // class KeyboardEvent

class MouseEvent : public DeviceEvent
{
public:
	MouseEvent() :
		DeviceEvent(Device::MOUSE),
		_event(Device::Mouse::UNKNOWN),
		_button(Device::Mouse::NULL_BUTTON)
	{}

	virtual ~MouseEvent()
	{}

public:
	inline virtual uint32_t getSize() const
	{
		return DeviceEvent::getSize()
			+ sizeof(this->_event)
			+ sizeof(this->_button)
			+ sizeof(this->_position);
	}

public:
	inline Device::Mouse::Event getEvent() const { return this->_event; }
	inline void setEvent(const Device::Mouse::Event value) { this->_event = value; }
	inline Device::Mouse::Button getButton() const { return this->_button; }
	inline void setButton(const Device::Mouse::Button value) { this->_button = value; }
	inline const Math::Vector3f& getPosition() const { return this->_position; }
	inline void setPosition(const Math::Vector3f &value) { this->_position = value; }

public:
	virtual byte_v serialize() const
	{
		byte_v data = DeviceEvent::serialize();
		data.insert(data.end(),
			((char*)&this->_event)[0],
			((char*)&this->_event)[sizeof(this->_event)]);
		data.insert(data.end(),
			((char*)&this->_button)[0],
			((char*)&this->_button)[sizeof(this->_button)]);
		data.insert(data.end(),
			((char*)&this->_position.x())[0],
			((char*)&this->_position.x())[sizeof(this->_position.x())]);
		data.insert(data.end(),
			((char*)&this->_position.y())[0],
			((char*)&this->_position.y())[sizeof(this->_position.y())]);
		data.insert(data.end(),
			((char*)&this->_position.z())[0],
			((char*)&this->_position.z())[sizeof(this->_position.z())]);
		return data;
	}

protected:
	Device::Mouse::Event _event;
	Device::Mouse::Button _button;
	Math::Vector3f _position;
}; // class MouseEvent

} // namespace Input
} // namespace Nous

#endif // NOUS_INPUT_EVENT_H
