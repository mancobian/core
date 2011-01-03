#include "OISDevice.h"

using namespace RSSD;
using namespace RSSD::Input;

///
/// OISKeyboard
///

void OISKeyboard::generateUnbufferedEvents(
	OIS::Keyboard *keyboard,
	Event::Stream &stream)
{
	static char keys[OISKeyboard::NUM_KEYS];
	keyboard->copyKeyStates(keys);

	KeyboardEvent *event = NULL;
	for (uint32_t i=0; i<OISKeyboard::NUM_KEYS; ++i)
	{
		event = new KeyboardEvent();
		event->setKeycode(keys[i]);
		if (keys[i])
			event->setEvent(Device::Keyboard::KEY_DOWN);
		else
			event->setEvent(Device::Keyboard::KEY_UP);
		stream.append(event);
	}
}

///
/// OISMouse
///

OISMouse::Button_m OISMouse::_buttons;

void OISMouse::initButtonMap()
{
	if (!OISMouse::_buttons.empty()) return;
	OISMouse::_buttons.insert(std::make_pair(OIS::MB_Left, Device::Mouse::LEFT_BUTTON));
	OISMouse::_buttons.insert(std::make_pair(OIS::MB_Right, Device::Mouse::RIGHT_BUTTON));
	OISMouse::_buttons.insert(std::make_pair(OIS::MB_Middle, Device::Mouse::MIDDLE_BUTTON));
	OISMouse::_buttons.insert(std::make_pair(OIS::MB_Button3, Device::Mouse::MISC_BUTTON_01));
	OISMouse::_buttons.insert(std::make_pair(OIS::MB_Button4, Device::Mouse::MISC_BUTTON_02));
	OISMouse::_buttons.insert(std::make_pair(OIS::MB_Button5, Device::Mouse::MISC_BUTTON_03));
	OISMouse::_buttons.insert(std::make_pair(OIS::MB_Button6, Device::Mouse::MISC_BUTTON_04));
	OISMouse::_buttons.insert(std::make_pair(OIS::MB_Button7, Device::Mouse::MISC_BUTTON_05));
}

Device::Mouse::Button OISMouse::toNativeType(const OIS::MouseButtonID id)
{
	if (OISMouse::_buttons.empty())
		OISMouse::initButtonMap();
	if (OISMouse::_buttons.find(id) != OISMouse::_buttons.end())
		return static_cast<Device::Mouse::Button>(OISMouse::_buttons[id]);
	return Device::Mouse::NULL_BUTTON;
}

void OISMouse::generateUnbufferedEvents(
	OIS::Mouse *mouse,
	Event::Stream &stream)
{
	MouseEvent *event = NULL;
	OISMouse::Button_m::iterator iter = OISMouse::_buttons.begin();
	OISMouse::Button_m::iterator end = OISMouse::_buttons.end();
	for (; iter != end; ++iter)
	{
		event = new MouseEvent();
		if (mouse->getMouseState().buttonDown(iter->first))
			event->setEvent(Device::Mouse::BUTTON_DOWN);
		else
			event->setEvent(Device::Mouse::BUTTON_UP);
		stream.append(event);
	}
}
