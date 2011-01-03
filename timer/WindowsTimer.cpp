#include "WindowsTimer.h"

using namespace RSSD;
using namespace RSSD::Core;

DECLARE_SINGLETON(RSSD::Core::WindowsTimer::Factory);

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
