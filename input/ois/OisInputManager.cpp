#include "input/ois/OisInputManager.h"
#include "input/ois/OisKeyboard.h"
#include "input/ois/OisMouse.h"

using namespace RSSD;
using namespace RSSD::Core;
using namespace RSSD::Core::Input;

OisInputManager::OisInputManager(const uint_t windowHandle) :
  mInputSystem(NULL)
{
  /// Prepare window handle for paramater list
  std::stringstream ss;
  ss << windowHandle;

  /// Create initialization parameter list
  OIS::ParamList params;
  params.insert(std::make_pair("WINDOW", ss.str()));

  /// Create input system
  this->mInputSystem = OIS::InputManager::createInputSystem(params);
}

OisInputManager::~OisInputManager()
{
  /// Destroy input system
  OIS::InputManager::destroyInputSystem(this->mInputSystem);
}

bool OisInputManager::update(const float_t elapsed)
{
  Manager::ItemList::iterator
    iter = this->_items.begin(),
    end = this->_items.end();
  for (; iter != end; ++iter)
  {
    (*iter)->update(elapsed);
  }
  return true;
}

Device* OisInputManager::createDevice(const uint32_t type)
{
  Device *device = NULL;

  if (device->getType() == Keyboard::TYPE) { device = this->createKeyboard(); }
  else if (device->getType() == Mouse::TYPE) { device = this->createMouse(); }
  else { assert (false && "Unrecognized device type."); }

  if (device) { this->add(device); }
  return device;
}

Device* OisInputManager::createKeyboard()
{
  Device *device = NULL;

  // If possible create a buffered keyboard
  // (note: if below line doesn't compile, try:  if (mInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0) {
  // if( mInputSystem->numKeyboards() > 0 ) {
  if (this->mInputSystem->getNumberOfDevices(OIS::OISKeyboard) > 0)
  {
    OIS::Object *object = this->mInputSystem->createInputObject(OIS::OISKeyboard, true);
    OIS::Keyboard *keyboard = reinterpret_cast<OIS::Keyboard*>(object);
    keyboard->setEventCallback(this);
    device = new OisKeyboard(keyboard);
  }
  return device;
}

Device* OisInputManager::createMouse()
{
  Device *device = NULL;

  if (this->mInputSystem->getNumberOfDevices(OIS::OISMouse) > 0)
  {
    OIS::Object *object = this->mInputSystem->createInputObject(OIS::OISMouse, true);
    OIS::Mouse *mouse = reinterpret_cast<OIS::Mouse*>(object);
    mouse->setEventCallback(this);
    device = new OisMouse(mouse);
  }
  return device;
}

bool OisInputManager::destroyDevice(Device *device)
{
  assert (this->remove(device));
  delete device;
  return true;
}

bool OisInputManager::keyPressed( const OIS::KeyEvent &e )
{
  return true;
}

bool OisInputManager::keyReleased( const OIS::KeyEvent &e )
{
  return true;
}

bool OisInputManager::mouseMoved( const OIS::MouseEvent &e )
{
  return true;
}

bool OisInputManager::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
  return true;
}

bool OisInputManager::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
  return true;
}

bool OisInputManager::povMoved( const OIS::JoyStickEvent &e, int pov )
{
  return true;
}

bool OisInputManager::axisMoved( const OIS::JoyStickEvent &e, int axis )
{
  return true;
}

bool OisInputManager::sliderMoved( const OIS::JoyStickEvent &e, int sliderID )
{
  return true;
}

bool OisInputManager::buttonPressed( const OIS::JoyStickEvent &e, int button )
{
  return true;
}

bool OisInputManager::buttonReleased( const OIS::JoyStickEvent &e, int button )
{
  return true;
}
