#include "PosixTimer.h"

using namespace RSSD;
using namespace RSSD::Core;
using namespace RSSD::Core::Utilities;
using namespace RSSD::Core::Utilities::Impl;

//!
// @class PosixTimer
// @todo Add errno error checking for each system call
///

PosixTimer::PosixTimer(params_t &params)
{
	this->reset();
}

PosixTimer::~PosixTimer()
{

}

void PosixTimer::start()
{
	gettimeofday(&this->_start, NULL);
}

void PosixTimer::stop()
{
	gettimeofday(&this->_stop, NULL);
}

void PosixTimer::reset()
{
	this->_start.tv_sec =
	this->_start.tv_usec =
	this->_stop.tv_sec =
	this->_stop.tv_usec = 0;
}

uint64_t PosixTimer::getMicroseconds() const
{
	uint64_t start = static_cast<uint64_t>(this->_start.tv_usec);
	start += static_cast<uint64_t>(this->_start.tv_sec) * Timer::USEC_PER_SEC;

	uint64_t stop = static_cast<uint64_t>(this->_stop.tv_usec);
	stop += static_cast<uint64_t>(this->_stop.tv_sec) * Timer::USEC_PER_SEC;

	return stop - start;
}
