///
/// @file OISDevice.h
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

#ifndef NOUS_INPUT_OISDEVICE_H
#define NOUS_INPUT_OISDEVICE_H

#include <core/Core.h>
#include <OIS/OIS.h>
#include <input/Device.h>
#include <event/Event.h>

namespace Nous {
namespace Input {

///
/// @class OISKeyboard
///

class OISKeyboard
{
public:
	static const uint32_t NUM_KEYS = 256;

public:
	typedef uint32_t KeyCode;
	TYPEDEF_CONTAINERS(KeyCode);
	typedef std::map<Device::Keyboard::Event, KeyCode_l> KeyboardState_m;

public:
	static void generateUnbufferedEvents(
		OIS::Keyboard *keyboard,
		Event::Stream &stream);
}; // class OISKeyboard

class OISMouse
{
public:
	typedef std::map<OIS::MouseButtonID, Device::Mouse::Button> Button_m;

public:
	static const uint32_t NUM_BUTTONS = Device::Mouse::MISC_BUTTON_05 + 1;

public:
	static Device::Mouse::Button toNativeType(
		const OIS::MouseButtonID id);
	static void generateUnbufferedEvents(
		OIS::Mouse *mouse,
		Event::Stream &stream);

protected:
	static void initButtonMap();

protected:
	static Button_m _buttons;
}; // class OISMouse

} // namespace Input
} // namespace Nous

#endif // NOUS_INPUT_OISDEVICE_H
