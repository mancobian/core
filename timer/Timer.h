///
/// @file Timer.h
/// @author Mancobian Poemandres
/// @license BSD License
///
/// Copyright (c) MMX by The Secret Design Collective
/// All rights reserved
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
///    * Redistributions of source code must retain the above copyright notice,
/// 		this list of conditions and the following disclaimer.
///    * Redistributions in binary form must reproduce the above copyright notice,
/// 		this list of conditions and the following disclaimer in the documentation
/// 		and/or other materials provided with the distribution.
///    * Neither the name of The Secret Design Collective nor the names of its
/// 		contributors may be used to endorse or promote products derived from
/// 		this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
/// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///

#ifndef NOUS_TIMER_TIMER_H
#define NOUS_TIMER_TIMER_H

#include "System"
#include "Pattern"

namespace RSSD {

class Timer
{
	public:
		static const uint64_t USEC_PER_SEC = 1000000;
		static const uint64_t USEC_PER_MSEC = 1000;

	public:
		Timer();
		virtual ~Timer();

	public:
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void reset() = 0;
		/// @note Elapsed time in integer microseconds
		/// @note 2^64 useconds = ~584554.431 years
		virtual uint64_t getMicroseconds() const = 0;

	public:
		virtual float64_t getMilliseconds() const
		{
			return static_cast<float64_t>(this->getMicroseconds())
				/ static_cast<float64_t>(USEC_PER_MSEC);
		}

		virtual float64_t getSeconds() const
		{
			return static_cast<float64_t>(this->getMicroseconds())
				/ static_cast<float64_t>(USEC_PER_SEC);
		}
}; // class Timer

typedef Pattern::Factory<Timer> TimerFactory;

class TimerManager :
	public Pattern::Singleton<TimerManager>,
	public Pattern::Manager<TimerFactory*>
{
	public:
		typedef Pattern::Manager<TimerFactory*> Manager_t;
		typedef Manager_t::Item_l TimerFactory_l;

	public:
		TimerManager();
		virtual ~TimerManager();

	public:
		TimerFactory* find(const uint32_t &type);
		Timer* createTimer(const System::Platform::Types type = System::Platform::UNKNOWN);

	protected:
		void createFactories();
		void destroyFactories();
}; // class TimerManager

} // namespace RSSD

#endif // NOUS_TIMER_TIMER_H
