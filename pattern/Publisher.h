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

#include "../System.h"
#include "Manager.h"

namespace rssd {
namespace pattern {
class Publication;
class Subscriber;
class Publisher;
}
}

namespace rssd {
namespace pattern {

class Publication
{
public:
	Publication() : _publisher(NULL) {}
	virtual ~Publication() {}

public:
	virtual inline Publisher* getPublisher() const { return this->_publisher; }
	virtual inline void setPublisher(Publisher *value) { this->_publisher = value; }

protected:
	Publisher *_publisher;
}; // class Publication

class Subscriber
{
public:
	virtual ~Subscriber();

public:
	virtual bool onSubscriptionPublished(Publication *publication) = 0;
}; // class Subscriber

class Publisher : virtual public pattern::Manager<Subscriber*>
{
public:
	typedef pattern::Manager<Subscriber*> Base;

public:
	virtual ~Publisher()
	{}

public:
	bool publish(Publication *publication)
	{
		// TODO: Re-factor to use std::for_each() algorithm
		Base::Item_l::iterator iter = this->_items.begin(),
			end = this->_items.end();
		while (iter != end)
		{
			Subscriber *subscriber = *iter++;
			if (!subscriber->onSubscriptionPublished(publication))
				return false;
		}
		return true;
	}
}; // class Publisher

} // namespace pattern
} // namespace rssd

#endif // RSSD_CORE_PATTERN_PUBLISHER_H
