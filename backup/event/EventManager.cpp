#include "EventManager.h"

using namespace RSSD;

DECLARE_SINGLETON(EventManager);

EventManager::EventManager() :
  Pattern::Singleton<EventManager>(),
  Pattern::Publisher<Event*>(),
  Pattern::Manager<Event*>()
{

}

EventManager::~EventManager()
{

}

uint32_t EventManager::getNumEvents() const
{
  return EventManager::Manager::size();
}

bool EventManager::addEvent(Event *event)
{
  return EventManager::Manager::add(event);
}

bool EventManager::removeEvent(Event *event)
{
  return EventManager::Manager::remove(event);
}

void EventManager::clearEvents()
{
  return EventManager::Manager::clear();
}
