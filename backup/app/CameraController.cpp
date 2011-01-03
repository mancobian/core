#include "CameraController.h"
#include <log/Log.h>
#include <input/DeviceEvent.h>
#include <input/InputManager.h>

using namespace Nous;
using namespace Nous::App;

CameraController::CameraController() :
	_is_turbo_enabled(false)
{
}

CameraController::~CameraController()
{
}

bool CameraController::enableTurbo(const Input::DeviceEvent &event)
{ this->_is_turbo_enabled = true; return true; }

bool CameraController::disableTurbo(const Input::DeviceEvent &event)
{ this->_is_turbo_enabled = false; return true; }

bool CameraController::moveForward(const Input::DeviceEvent &event)
{ this->move(static_cast<Directions>(FORWARD)); return true; }

bool CameraController::moveBackward(const Input::DeviceEvent &event)
{ this->move(static_cast<Directions>(BACKWARD)); return true; }

bool CameraController::moveLeft(const Input::DeviceEvent &event)
{ this->move(static_cast<Directions>(LEFT)); return true; }

bool CameraController::moveRight(const Input::DeviceEvent &event)
{ this->move(static_cast<Directions>(RIGHT)); return true; }

bool CameraController::moveDown(const Input::DeviceEvent &event)
{ this->move(static_cast<Directions>(DOWN)); return true; }

bool CameraController::moveUp(const Input::DeviceEvent &event)
{ this->move(static_cast<Directions>(UP)); return true; }

bool CameraController::rotateForward(const Input::DeviceEvent &event)
{ this->rotate(static_cast<Directions>(FORWARD)); return true; }

bool CameraController::rotateBackward(const Input::DeviceEvent &event)
{ this->rotate(static_cast<Directions>(BACKWARD)); return true; }

bool CameraController::rotateLeft(const Input::DeviceEvent &event)
{ this->rotate(static_cast<Directions>(LEFT)); return true; }

bool CameraController::rotateRight(const Input::DeviceEvent &event)
{ this->rotate(static_cast<Directions>(RIGHT)); return true; }

bool CameraController::rotateDown(const Input::DeviceEvent &event)
{ this->rotate(static_cast<Directions>(DOWN)); return true; }

bool CameraController::rotateUp(const Input::DeviceEvent &event)
{ this->rotate(static_cast<Directions>(UP)); return true; }

void CameraController::CameraController::move(const Directions direction, const float32_t value)
{
	switch (static_cast<uint32_t>(direction))
	{
		case CameraController::FORWARD: { this->_translation.z() -= value; break; }
		case CameraController::BACKWARD: { this->_translation.z() += value; break; }
		case CameraController::LEFT: { this->_translation.x() -= value; break; }
		case CameraController::RIGHT: { this->_translation.x() += value; break; }
		case CameraController::DOWN: { this->_translation.y() -= value; break; }
		case CameraController::UP: { this->_translation.y() += value; break; }
		case CameraController::UNKNOWN:
		default: break;
	}
}

void CameraController::CameraController::rotate(const Directions direction, const float32_t value)
{
	switch (static_cast<uint32_t>(direction))
	{
		// Roll, rotation about the z-axis
		case CameraController::FORWARD: { this->_rotation.z() -= value; break; }
		case CameraController::BACKWARD: { this->_rotation.z() += value; break; }
		// Yaw, rotation about the y-axis
		case CameraController::LEFT: { this->_rotation.y() += value; break; }
		case CameraController::RIGHT: { this->_rotation.y() -= value; break; }
		// Pitch, rotation about the x-axis
		case CameraController::DOWN: { this->_rotation.x() -= value; break; }
		case CameraController::UP: { this->_rotation.x() += value; break; }
		case CameraController::UNKNOWN:
		default: break;
	}
}
