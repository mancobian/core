///
/// @file CameraController.h
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

#ifndef NOUS_APP_CAMERACONTROLLER_H
#define NOUS_APP_CAMERACONTROLLER_H

#include <core/Core.h>

namespace Nous {
namespace Input {
class DeviceEvent;
} // namespace Input
} // namespace Nous

namespace Nous {
namespace App {

class CameraController
{
public:
	enum Directions
	{
		UNKNOWN = 0,
		POSITIVE_X,
		NEGATIVE_X,
		POSITIVE_Y,
		NEGATIVE_Y,
		POSITIVE_Z,
		NEGATIVE_Z,
	}; // enum Directions

public:
	static const uint32_t FORWARD = NEGATIVE_Z;
	static const uint32_t BACKWARD = POSITIVE_Z;
	static const uint32_t LEFT = NEGATIVE_X;
	static const uint32_t RIGHT = POSITIVE_X;
	static const uint32_t UP = NEGATIVE_Y;
	static const uint32_t DOWN = POSITIVE_Y;
	static const uint32_t MOVE_RATE = 2.5f;
	static const uint32_t MOVE_MULTIPLIER = 5.0f;
	static const uint32_t ROTATE_RATE = 2.0f;

public:
	CameraController();
	virtual ~CameraController();

public:
	virtual bool update(const float32_t elapsed) = 0;

public:
	bool enableTurbo(const Input::DeviceEvent &event);
	bool disableTurbo(const Input::DeviceEvent &event);
	bool moveForward(const Input::DeviceEvent &event);
	bool moveBackward(const Input::DeviceEvent &event);
	bool moveLeft(const Input::DeviceEvent &event);
	bool moveRight(const Input::DeviceEvent &event);
	bool moveDown(const Input::DeviceEvent &event);
	bool moveUp(const Input::DeviceEvent &event);
	bool rotateForward(const Input::DeviceEvent &event);
	bool rotateBackward(const Input::DeviceEvent &event);
	bool rotateLeft(const Input::DeviceEvent &event);
	bool rotateRight(const Input::DeviceEvent &event);
	bool rotateDown(const Input::DeviceEvent &event);
	bool rotateUp(const Input::DeviceEvent &event);

protected:
	void move(const Directions direction, const float32_t value = CameraController::MOVE_RATE);
	void rotate(const Directions direction, const float32_t value = CameraController::ROTATE_RATE);

protected:
	bool _is_turbo_enabled;
	Math::Vector3f _translation,
		_rotation;
}; // class CameraController

} // namespace App
} // namespace Nous

#endif // NOUS_APP_CAMERACONTROLLER_H
