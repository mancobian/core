#include "WindowsTimer.h"

using namespace RSSD;

template<> WindowsTimerFactory* Pattern::Singleton<WindowsTimerFactory>::_instance = NULL;

TimerFactory::Product* WindowsTimerFactory::createImpl()
{
	return new WindowsTimer();
}

WindowsTimer::WindowsTimer()
{

}

WindowsTimer::~WindowsTimer()
{

}

void WindowsTimer::start()
{

}

void WindowsTimer::stop()
{

}

void WindowsTimer::reset()
{

}

uint64_t WindowsTimer::getMicroseconds() const
{
	return static_cast<uint64_t>(0);
}
