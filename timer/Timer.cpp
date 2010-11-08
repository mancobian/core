#include "Timer.h"
#include "PosixTimer.h"
#include "WindowsTimer.h"

using namespace RSSD;

///
/// @class TimerManager
///

template<> TimerManager* Pattern::Singleton<TimerManager>::_instance = NULL;

TimerManager::TimerManager() :
	Pattern::Manager<TimerFactory*>()
{
	this->createFactories();
}

TimerManager::~TimerManager()
{
	this->destroyFactories();
}

void TimerManager::createFactories()
{
	this->add(new PosixTimerFactory());
	this->add(new WindowsTimerFactory());
}

void TimerManager::destroyFactories()
{
	this->clear();
}

TimerFactory* TimerManager::find(const uint32_t &type)
{
	TimerFactory_l::iterator iter = this->_items.begin(),
		end = this->_items.end();
	while (iter != end)
	{
		TimerFactory *factory = *iter++;
		if (*factory == type)
			return factory;
	}
	return NULL;
}

Timer* TimerManager::createTimer(const System::Platform::Types type)
{
	return NULL;
}

Timer::Timer()
{
}

Timer::~Timer()
{
}
