///
/// @file Publisher.h
/// @author Mancobian Poemandres
/// @license BSD License
///
/// Copyright (c) MMX by Royal Society of Secret Design
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
///    * Neither the name of Royal Society of Secret Design nor the names of its
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

#ifndef RSSD_CORE_PATTERN_PUBLISHER_H
#define RSSD_CORE_PATTERN_PUBLISHER_H

#include "System"
#include "Manager.h"
#include "system/Memory.h"

namespace RSSD {
namespace Core {
namespace Pattern {

///
/// @note typename T The object with which to update registered subscribers.
///
template <typename T>
class Publisher
{
public:
  /// @todo Consider if the subscriber notification callback can be
  ///  templated such that the target callback function may be supplied
  ///  by a user and you can simply use the function call overloaded
  ///  operator to invoke the target callback function.
	class Subscriber
	{
	public:
	  typedef std::tr1::shared_ptr<Subscriber> Pointer;
	  typedef std::tr1::weak_ptr<Subscriber> WeakPointer;

	  virtual ~Subscriber() {}
	  virtual void onNotification(const T &publication) = 0;
	}; // class Subscriber

	Publisher();
	virtual ~Publisher();
	void publish(const T &publication);
	bool hasSubscriber(Subscriber *subscriber);
	bool registerSubscriber(Subscriber *subscriber);
	bool unregisterSubscriber(Subscriber *subscriber);

protected:
	typedef Pattern::Manager<typename Publisher<T>::Subscriber::WeakPointer> SubscriberManager;

	typename SubscriberManager::Pointer mSubscriberManager;
}; // class Publisher

///
/// Includes
///

#include "Publisher-inl.h"

} // namespace Pattern
} // namespace Core
} // namespace RSSD

template <typename T>
bool operator ==(
    const std::tr1::weak_ptr<typename RSSD::Core::Pattern::Publisher<T>::Subscriber>& a,
    const std::tr1::weak_ptr<typename RSSD::Core::Pattern::Publisher<T>::Subscriber>& b);

#endif // RSSD_CORE_PATTERN_PUBLISHER_H
