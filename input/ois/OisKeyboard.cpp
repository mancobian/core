#include "input/ois/OisKeyboard.h"

using namespace RSSD;
using namespace RSSD::Core;
using namespace RSSD::Core::Input;
using namespace RSSD::Core::Input::Impl;

OisKeyboard::OisKeyboard(params_t &params) :
  OisDevice<OIS::Keyboard>(params)
{
  this->mDeviceHandle->setEventCallback(this);
}

OisKeyboard::~OisKeyboard()
{
}

bool OisKeyboard::update(const float32_t elapsed)
{
  return true;
}

bool OisKeyboard::keyPressed(const OIS::KeyEvent &e)
{
  return true;
}

bool OisKeyboard::keyReleased(const OIS::KeyEvent &e)
{
  return true;
}
