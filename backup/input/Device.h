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

#include <core/Core.h>

#ifndef NOUS_INPUT_DEVICE_H
#define NOUS_INPUT_DEVICE_H

namespace Nous {
namespace Input {

class Device
{
public:
	enum Type
	{
		UNKNOWN = 0,
		MOUSE,
		KEYBOARD,
		GAMEPAD,
		TOUCH,
		SPEECH
	}; // enum Type

public:
	class Keyboard
	{
	public:
		enum Event
		{
			UNKNOWN = 0,
			KEY_PRESS,
			KEY_DOWN,
			KEY_RELEASE,
			KEY_UP
		}; // enum Event
	}; // class Keyboard

	class Mouse
	{
	public:
		enum Event
		{
			UNKNOWN = 0,
			BUTTON_PRESS,
			BUTTON_DOWN,
			BUTTON_RELEASE,
			BUTTON_UP,
			SCROLL,
			MOVE
		}; // enum Event

		enum Button
		{
			NULL_BUTTON = 0,
			LEFT_BUTTON,
			RIGHT_BUTTON,
			MIDDLE_BUTTON,
			MISC_BUTTON_01,
			MISC_BUTTON_02,
			MISC_BUTTON_03,
			MISC_BUTTON_04,
			MISC_BUTTON_05
		}; // enum Button
	}; // class Mouse
}; // class Device

} // namespace Input
} // namespace Nous

#endif // NOUS_INPUT_DEVICE_H
