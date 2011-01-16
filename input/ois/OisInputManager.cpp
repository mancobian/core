#include "input/ois/OisInputManager.h"
#include "input/Device.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "input/ois/OisKeyboard.h"
#include "input/ois/OisMouse.h"

using namespace RSSD;
using namespace RSSD::Core;
using namespace RSSD::Core::Input;
using namespace RSSD::Core::Input::Impl;

OisInputManager::OisInputManager(OisInputManager::Traits::WindowHandleType windowHandle) :
  mInputSystem(NULL)
{
  if (windowHandle) { this->create(windowHandle); }
}

OisInputManager::~OisInputManager()
{
  this->destroy();
}

OIS::Type OisInputManager::toOisType(const uint32_t type)
{
  switch (type)
  {
    case Device::Types::MOUSE: { return OIS::OISMouse; }
    case Device::Types::KEYBOARD: { return OIS::OISKeyboard; }
    default: { break; }
  }
  return OIS::OISUnknown;
}

bool OisInputManager::isDeviceAvailable(const OIS::Type type) const
{
  assert ((this->mInputSystem->getNumberOfDevices(type) > 0)  && "No devices detected of the given type.");
  return true;
}

bool OisInputManager::create(const Traits::WindowHandleType windowHandle)
{
  /// Error checking
  assert (this->mInputSystem == NULL);

  /// Prepare window handle for paramater list
  std::stringstream ss;
  ss << windowHandle;

  /// Create initialization parameter list
  OIS::ParamList params;
  params.insert(std::make_pair("WINDOW", ss.str()));

  /// Create input system
  this->mInputSystem = OIS::InputManager::createInputSystem(params);
  return true;
}

bool OisInputManager::destroy()
{
  /// Destroy input system
  OIS::InputManager::destroyInputSystem(this->mInputSystem);
  this->mInputSystem = NULL;
  return true;
}

bool OisInputManager::update(const float_t elapsed)
{
  DeviceManager::ItemList &items = this->getItems();
  DeviceManager::ItemList::iterator
    iter = items.begin(),
    end = items.end();
  for (; iter != end; ++iter)
  {
    (*iter)->update(elapsed);
  }
  return true;
}

Device* OisInputManager::createDevice(const uint32_t type)
{
  return this->createDevice(type, true);
}

Device* OisInputManager::createDevice(const uint32_t type, const bool isBuffered)
{
  const OIS::Type oisType = OisInputManager::toOisType(type);
  if (!this->isDeviceAvailable(oisType)) { return NULL; }

  Traits::DeviceHandleType deviceHandle = this->mInputSystem->createInputObject(oisType, isBuffered);
  params_t params;
  params.insert(std::make_pair(Device::Params::DEVICE_HANDLE, deviceHandle));
  Device *device = Device::Manager::getPointer()->getFactory(type)->create(params);

  this->add(device);
  return device;
}

bool OisInputManager::destroyDevice(Input::Device *device)
{
  if (!device) { return false; }
  this->remove(device);

  Device::Manager::getPointer()->getFactory(device->getType())->destroy(device);
  return true;
}

/*
bool InputManager::Impl::povMoved( const Ois::JoyStickEvent &e, int pov )
{
  return true;
}

bool InputManager::Impl::axisMoved( const Ois::JoyStickEvent &e, int axis )
{
  return true;
}

bool InputManager::Impl::sliderMoved( const Ois::JoyStickEvent &e, int sliderID )
{
  return true;
}

bool InputManager::Impl::buttonPressed( const Ois::JoyStickEvent &e, int button )
{
  return true;
}

bool InputManager::Impl::buttonReleased( const Ois::JoyStickEvent &e, int button )
{
  return true;
}
*/

