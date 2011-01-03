#if USE_EXPORT_KEYWORD
#include "input/InputManager.h"
#endif

#if 0
bool InputManager::addLookup(
	InputListener::Delegate &delegate,
	const DeviceEvent &device_event)
{
#if 0
	{
		LOG ("Registering delegate <" << delegate.get().get() << "> "
			<< "with device event: "
			<< device_event.toString(), "INPUT");
		LOG ("Currently registered device events:", "INPUT");
		ListenerLookup_m::iterator iter = this->_lookups.begin();
		ListenerLookup_m::iterator end = this->_lookups.end();
		for (; iter != end; ++iter)
		{
			LOG ("\t => " << iter->first.toString(), "INPUT");
		}
	}
#endif
	// Create a new lookup pair if the device event isn't
	// already present in the lookup registry
	if (this->_lookups.find(device_event) == this->_lookups.end())
		this->_lookups.insert(std::make_pair(device_event, Delegate_l()));

	// Confirm the delegate is not already registered
	// for this device event
	Delegate_l::iterator iter = std::find(
		this->_lookups[device_event].begin(),
		this->_lookups[device_event].end(),
		delegate);
	if (iter != this->_lookups[device_event].end())
		return false;

	// Insert new lookup delegate for this device event
	this->_lookups[device_event].push_back(delegate);
	return true;
}

bool InputManager::removeLookup(
	InputListener::Delegate &delegate,
	const DeviceEvent &device_event)
{
	// Confirm that a lookup pair exists for this device event
	if (this->_lookups.find(device_event) == this->_lookups.end())
		return false;

	// Confirm the delegate is already registered
	// for this device event
	Delegate_l::iterator iter = std::find(
		this->_lookups[device_event].begin(),
		this->_lookups[device_event].end(),
		delegate);
	if (iter == this->_lookups[device_event].end())
		return false;

	// Insert new lookup delegate for this device event
	this->_lookups[device_event].remove(delegate);
	return true;
}

InputManager::Delegate_l* InputManager::getDelegates(const DeviceEvent &device_event)
{
#if 0
	{
		LOG ("Searching for delegates registered with device event: "
			<< device_event.toString(), "INPUT");
		LOG ("Currently registered device events:", "INPUT");
		ListenerLookup_m::iterator iter = this->_lookups.begin();
		ListenerLookup_m::iterator end = this->_lookups.end();
		for (; iter != end; ++iter)
		{
			LOG ("\t => " << iter->first.toString(), "INPUT");
		}
	}
#endif
	if (this->_lookups.find(device_event) == this->_lookups.end())
		return NULL;
	return &(this->_lookups[device_event]);
}

DeviceEvent_l InputManager::getDeviceEvents(const InputListener::Delegate &delegate)
{
	DeviceEvent_l events;
	if (this->_listeners.find(delegate) != this->_listeners.end())
	{
		DeviceEvent_m::iterator iter = this->_listeners[delegate].begin();
		DeviceEvent_m::iterator end = this->_listeners[delegate].end();
		for (; iter != end; ++iter)
			events.push_back(iter->first);
	}
	return events;
}

InputListener::ListenerSignature::Event* InputManager::getEvent(
	InputListener::Delegate &delegate,
	const DeviceEvent &device_event,
	const bool create_missing_keys)
{
	// Get the device event map
	Listener_m::iterator listener_iter = this->_listeners.find(delegate);
	if (listener_iter == this->_listeners.end())
	{
		if (create_missing_keys)
		{
			std::pair<Listener_m::iterator, bool> result = this->_listeners.insert(
				std::make_pair(delegate, DeviceEvent_m()));
		}
		else return NULL;
	}
	DeviceEvent_m &events = this->_listeners[delegate];

#if 0
	DeviceEvent DEBUG_MOUSE_PRESS(Device::MOUSE, Mouse::BUTTON_PRESS);
	if (device_event == DEBUG_MOUSE_PRESS)
	{
		LOG ("Searching for MOUSE PRESS in delegate <" << delegate.get().get() << ">", "INPUT");
		DeviceEvent_m::iterator iter = events.begin();
		DeviceEvent_m::iterator end = events.end();
		for (; iter != end; ++iter)
		{
			LOG ("\t=> Device event: " << iter->first.toString(), "INPUT");
		}
	}
#endif

	// Get the event object
	DeviceEvent_m::iterator event_iter = events.find(device_event);
	if (event_iter == events.end())
	{
		if (create_missing_keys)
		{
			events.insert(std::make_pair(device_event, InputListener::ListenerSignature::Event()));
		}
		else return NULL;
	}
	return &(events[device_event]);
}

bool InputManager::hasListener(InputListener::Delegate &delegate, const DeviceEvent &device_event)
{
	InputListener::ListenerSignature::Event *event = this->getEvent(delegate, device_event);
	if (!event) return false;
	return event->has(delegate);
}

bool InputManager::addListener(InputListener::Delegate &delegate, const DeviceEvent &device_event)
{
#if 0
	{
		if (device_event.getValue().Integral == 0)
		{
			LOG ("Registering device_event with delegate <"
				<< delegate.get().get() << ">: "
				<< device_event.toString(), "INPUT");
			LOG ("Currently registered delegates:", "INPUT");
			Listener_m::iterator iter = this->_listeners.begin();
			Listener_m::iterator end = this->_listeners.end();
			for (; iter != end; ++iter)
			{
				LOG ("\t => " << iter->first.get().get(), "INPUT");
			}
		}
	}
#endif
	this->removeListener(delegate, device_event);
	InputListener::ListenerSignature::Event *event = this->getEvent(delegate, device_event, true);
	this->addLookup(delegate, device_event);
	return event->add(delegate);
}

bool InputManager::removeListener(InputListener::Delegate &delegate, const DeviceEvent &device_event)
{
	InputListener::ListenerSignature::Event *event = this->getEvent(delegate, device_event);
	if (!event)
	{
		LOG ("WARNING: Failed to remove listener for device event: " << device_event.toString(), "INPUT");
		return false;
	}
	this->_listeners.erase(delegate);
	this->removeLookup(delegate, device_event);
	return event->remove(delegate);
}

bool InputManager::updateListeners(const DeviceEvent &device_event)
{
	// Update selective event listeners
	// LOG ("Updating selective listeners for device event: " << device_event.toString(), "INPUT");
	bool result = true;
	Delegate_l *delegates = this->getDelegates(device_event);
	if (delegates)
	{
		Delegate_l::iterator iter = delegates->begin();
		Delegate_l::iterator end = delegates->end();
		for (; iter != end; ++iter)
		{
			InputListener::ListenerSignature::Event *event = this->getEvent(*iter, device_event);
			if (!event) result &= false;
			else result &= event->raise(device_event);
		}
	}

	// Update promiscuous event listeners
	// LOG ("Updating promiscuous listeners for device event: " << device_event.toString(), "INPUT");
	DeviceEvent clone_event(device_event);
	clone_event.getValue().clear();
	delegates = this->getDelegates(clone_event);
	if (delegates)
	{
#if 0
		LOG ("Updating <" << delegates->size() << "> promiscuous listeners for "
			<< "device event: " << clone_event.toString(), "INPUT");
#endif
		Delegate_l::iterator iter = delegates->begin();
		Delegate_l::iterator end = delegates->end();
		for (; iter != end; ++iter)
		{
			InputListener::ListenerSignature::Event *event = this->getEvent(*iter, clone_event);
			if (!event) result &= false;
			else result &= event->raise(device_event);
		}
	}
	else
	{
#if 0
		LOG ("Failed to find any promiscuous listeners for device event: "
			<< clone_event.toString(), "INPUT");
#endif
		return false;
	}

	return result;
}

void InputManager::clearListeners()
{
	this->_listeners.clear();
}

EXPORT template <typename FUNCTION>
void InputManager::addKeyboardListener(
	FUNCTION function,
	const uint32_t event_type,
	const DeviceEvent::Value event_value)
{
	Input::DeviceEvent device_event(Device::KEYBOARD, event_type, event_value);
	InputListener::Delegate delegate(function);
	this->addListener(delegate, device_event);
}

EXPORT template <typename FUNCTION>
void InputManager::removeKeyboardListener(
	FUNCTION function,
	const uint32_t event_type,
	const DeviceEvent::Value event_value)
{
	Input::DeviceEvent device_event(Device::KEYBOARD, event_type, event_value);
	InputListener::Delegate delegate(function);
	this->removeListener(delegate, device_event);
}

EXPORT template <typename CLASS, typename METHOD>
void InputManager::addKeyboardListener(
	CLASS *instance,
	METHOD method,
	const uint32_t event_type,
	const DeviceEvent::Value event_value)
{
	Input::DeviceEvent device_event(Device::KEYBOARD, event_type, event_value);
	InputListener::Delegate delegate(instance, method);
	this->addListener(delegate, device_event);
}

EXPORT template <typename CLASS, typename METHOD>
void InputManager::removeKeyboardListener(
	CLASS *instance,
	METHOD method,
	const uint32_t event_type,
	const DeviceEvent::Value event_value)
{
	Input::DeviceEvent device_event(Device::KEYBOARD, event_type, event_value);
	InputListener::Delegate delegate(instance, method);
	this->removeListener(delegate, device_event);
}

EXPORT template <typename FUNCTION>
void InputManager::addMouseListener(
	FUNCTION function,
	const uint32_t event_type,
	const DeviceEvent::Value event_value)
{
	Input::DeviceEvent device_event(Device::MOUSE, event_type, event_value);
	InputListener::Delegate delegate(function);
	this->addListener(delegate, device_event);
}

EXPORT template <typename FUNCTION>
void InputManager::removeMouseListener(
	FUNCTION function,
	const uint32_t event_type,
	const DeviceEvent::Value event_value)
{
	Input::DeviceEvent device_event(Device::MOUSE, event_type, event_value);
	InputListener::Delegate delegate(function);
	this->removeListener(delegate, device_event);
}

EXPORT template <typename CLASS, typename METHOD>
void InputManager::addMouseListener(
	CLASS *instance,
	METHOD method,
	const uint32_t event_type,
	const DeviceEvent::Value event_value)
{
	Input::DeviceEvent device_event(Device::MOUSE, event_type, event_value);
	InputListener::Delegate delegate(instance, method);
	this->addListener(delegate, device_event);
}

EXPORT template <typename CLASS, typename METHOD>
void InputManager::removeMouseListener(
	CLASS *instance,
	METHOD method,
	const uint32_t event_type,
	const DeviceEvent::Value event_value)
{
	Input::DeviceEvent device_event(Device::MOUSE, event_type, event_value);
	InputListener::Delegate delegate(instance, method);
	this->removeListener(delegate, device_event);
}

#endif
