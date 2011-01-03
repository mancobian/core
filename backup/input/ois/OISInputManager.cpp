#include <input/ois/OISInputManager.h>
#include <sstream>
#include <log/Log.h>
#include <input/Event.h>
#include <input/ois/OISDevice.h>

using namespace Nous;
using namespace Nous::Input;

///
/// OISInputManager
///

template<> Nous::Input::OISInputManager* Pattern::Singleton<Nous::Input::OISInputManager>::_instance = 0;

OISInputManager::OISInputManager(const uint32_t hwnd,
	const uint32_t width,
	const uint32_t height) :
	InputManager()
{
	this->create(hwnd, width, height);
}

OISInputManager::~OISInputManager()
{
	this->destroy();
}

void OISInputManager::create()
{
	this->create(0,0,0);
}

void OISInputManager::create(
	const uint32_t hwnd,
	const uint32_t width,
	const uint32_t height)
{
	std::stringstream strhwnd;
	strhwnd << hwnd;

	// Customize window attributes
	OIS::ParamList paramList;
	paramList.insert(OIS::ParamList::value_type("WINDOW", strhwnd.str().c_str()));
#if defined OIS_WIN32_PLATFORM
	paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
	paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
	paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
	paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
#elif defined OIS_LINUX_PLATFORM
	paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
	paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
#endif

	// Create input manager
	this->_input_manager = OIS::InputManager::createInputSystem(paramList);

	// Create keyboard device(s)
#if (OIS_VERSION_MAJOR >= 1) && (OIS_VERSION_MINOR < 2)
	uint32_t num_keyboards = this->_input_manager->numKeyboards();
#else
	uint32_t num_keyboards = this->_input_manager->getNumberOfDevices(OIS::OISKeyboard);
	NLOG ("(OIS) Num keyboards: " << num_keyboards, "INPUT");
#endif
	for (uint32_t i=0; i<num_keyboards; ++i)
	{
		NLOG ("(OIS) Creating keyboard: " << i, "INPUT");
		OIS::Object *device = this->_input_manager->createInputObject(OIS::OISKeyboard, true);
		OIS::Keyboard *keyboard = static_cast<OIS::Keyboard*>(device);
		keyboard->setEventCallback(this);
		this->_keyboards.push_back(keyboard);
	}

	// Create mouse device(s)
#if (OIS_VERSION_MAJOR >= 1) && (OIS_VERSION_MINOR < 2)
	uint32_t num_mice = this->_input_manager->numMice();
#else
	uint32_t num_mice = this->_input_manager->getNumberOfDevices(OIS::OISMouse);
	NLOG ("(OIS) Num mice: " << num_mice, "INPUT");
#endif
	for (uint32_t i=0; i<num_mice; ++i)
	{
		NLOG ("(OIS) Creating mouse: " << i, "INPUT");
		OIS::Object *device = this->_input_manager->createInputObject(OIS::OISKeyboard, true);
		OIS::Mouse *mouse = static_cast<OIS::Mouse*>(device);
		mouse->setEventCallback(this);
		this->_mice.push_back(mouse);
	}
}

void OISInputManager::destroy()
{
	// Destroy keyboard device(s)
	OIS::KeyboardManager_l::iterator keyboard_iter = this->_keyboards.begin();
	OIS::KeyboardManager_l::iterator keyboard_end = this->_keyboards.end();
	for (; keyboard_iter != keyboard_end; ++keyboard_iter)
		this->_input_manager->destroyInputObject(*keyboard_iter);
	this->_keyboards.clear();

	// Destroy mouse device(s)
	OIS::MouseManager_l::iterator mouse_iter = this->_mice.begin();
	OIS::MouseManager_l::iterator mouse_end = this->_mice.end();
	for (; mouse_iter != mouse_end; ++mouse_iter)
		this->_input_manager->destroyInputObject(*mouse_iter);
	this->_mice.clear();

	// Destroy input manager
	OIS::InputManager::destroyInputSystem(this->_input_manager);
}

///
/// @todo There is currently no support for promiscuous listeners
/// to "unbuffered" device events like Keyboard::KEY_UP or Mouse::BUTTON_DOWN;
/// how should this be handled?
///

bool OISInputManager::update(const float32_t elapsed)
{
	// Update keyboard device(s)
	OIS::KeyboardManager_l::iterator keyboard_iter = this->_keyboards.begin();
	OIS::KeyboardManager_l::iterator keyboard_end = this->_keyboards.end();
	for (; keyboard_iter != keyboard_end; ++keyboard_iter)
	{
		// Update device state for processing
		OIS::Keyboard *keyboard = *keyboard_iter;
		keyboard->capture();

		// Generate unbuffered keyboard events
		OISKeyboard::generateUnbufferedEvents(
			keyboard, *this);
	}

	// Update mouse device(s)
	OIS::MouseManager_l::iterator mouse_iter = this->_mice.begin();
	OIS::MouseManager_l::iterator mouse_end = this->_mice.end();
	for (; mouse_iter != mouse_end; ++mouse_iter)
	{
		OIS::Mouse *mouse = *mouse_iter;
		mouse->capture();

		// Generate unbuffered keyboard events
		OISMouse::generateUnbufferedEvents(
			mouse, *this);
	}
	return true;
}

bool OISInputManager::keyPressed(const OIS::KeyEvent &key_event)
{
	// Get the associated input device
	OIS::Keyboard *keyboard = const_cast<OIS::Keyboard*>(
		static_cast<const OIS::Keyboard*>(key_event.device));
	if (std::find(
		this->_keyboards.begin(),
		this->_keyboards.end(),
		keyboard) == this->_keyboards.end())
		return false;

	// Create input event
	KeyboardEvent event;
	event.setEvent(Device::Keyboard::KEY_PRESS);
	event.setKeycode(key_event.key);
	return this->append(&event);
}

bool OISInputManager::keyReleased(const OIS::KeyEvent &key_event)
{
	// Get the associated input device
	OIS::Keyboard *keyboard = const_cast<OIS::Keyboard*>(
		static_cast<const OIS::Keyboard*>(key_event.device));
	if (std::find(
		this->_keyboards.begin(),
		this->_keyboards.end(),
		keyboard) == this->_keyboards.end())
		return false;

	// Create input event
	KeyboardEvent event;
	event.setEvent(Device::Keyboard::KEY_RELEASE);
	event.setKeycode(key_event.key);
	return this->append(&event);
}

bool OISInputManager::mouseMoved(const OIS::MouseEvent &mouse_event)
{
	// Get the associated input device
	OIS::Mouse *mouse = const_cast<OIS::Mouse*>(
		static_cast<const OIS::Mouse*>(mouse_event.device));
	if (std::find(
		this->_mice.begin(),
		this->_mice.end(),
		mouse) == this->_mice.end())
		return false;

	Math::Vector3f position(
		mouse_event.state.X.abs,
		mouse_event.state.Y.abs,
		mouse_event.state.Z.abs);

	// Create input event
	MouseEvent event;
	event.setEvent(Device::Mouse::MOVE);
	event.setPosition(position);
	return this->append(&event);
}

bool OISInputManager::mousePressed(const OIS::MouseEvent &mouse_event, OIS::MouseButtonID id)
{
	// Get the associated input device
	OIS::Mouse *mouse = const_cast<OIS::Mouse*>(
		static_cast<const OIS::Mouse*>(mouse_event.device));
	if (std::find(
		this->_mice.begin(),
		this->_mice.end(),
		mouse) == this->_mice.end())
		return false;

	MouseEvent event;
	event.setEvent(Device::Mouse::BUTTON_PRESS);
	event.setButton(OISMouse::toNativeType(id));
	return this->append(&event);
}

bool OISInputManager::mouseReleased(const OIS::MouseEvent &mouse_event, OIS::MouseButtonID id)
{
	// Get the associated input device
	OIS::Mouse *mouse = const_cast<OIS::Mouse*>(
		static_cast<const OIS::Mouse*>(mouse_event.device));
	if (std::find(
		this->_mice.begin(),
		this->_mice.end(),
		mouse) == this->_mice.end())
		return false;

	MouseEvent event;
	event.setEvent(Device::Mouse::BUTTON_RELEASE);
	event.setButton(OISMouse::toNativeType(id));
	return this->append(&event);
}
