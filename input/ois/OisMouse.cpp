#include "input/ois/OisMouse.h"

using namespace RSSD;
using namespace RSSD::Core;
using namespace RSSD::Core::Input;
using namespace RSSD::Core::Input::Impl;

///
/// @class State
///

OisMouse::State::State()
{
  /// Initialize mouse button states
  for (uint32_t i = 0; i < Mouse::Button::COUNT; ++i)
  {
    this->mButtons.insert(std::make_pair(i, 0));
  }
}

OisMouse::State::~State()
{
}

///
/// @class Impl
///

OisMouse::OisMouse(params_t &params) :
  OisDevice<OIS::Mouse>(params)
{
  this->mDeviceHandle->setEventCallback(this);
}

OisMouse::~OisMouse()
{

}

bool OisMouse::update(const float32_t elapsed)
{
  return true;
}

bool OisMouse::mouseMoved( const OIS::MouseEvent &e )
{
  return true;
}

bool OisMouse::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
  return true;
}

bool OisMouse::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
  return true;
}
