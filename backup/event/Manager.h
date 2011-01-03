///
/// @file Manager.h
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

#ifndef NOUS_EVENT_MANAGER_H
#define NOUS_EVENT_MANAGER_H

#if 0
/*
An EVENT MANAGER has an EVENT STREAM.

The EVENT MANAGER is a higher-level object and the EVENT
PRODUCERS are lower-level objects. The MANAGER should PROCESS
the aggregate event stream and be responsible for INVOKING
ROUTINES in lower-level objects based on EVENTS. Lower-level
EVENT PRODUCERs should not be aware of other objects at their
same level; it is sensible for the MANAGER to be aware of the
lower-level objects, however. This prevents CROSS-POLLINATION
OF DATA so that an object that reacts to particular events
does not need to know about the technical details of those
events or their associated EVENT PRODUCER.

The MANAGER needs to allow PRODUCERS of events
to REGISTER, such that on EACH FRAME, the MANAGER will know
where to look to AGGREGATE all relevant EVENT STREAMS.

On EACH FRAME, the MANAGER must APPEND the EVENT STREAM of
REGISTERED EVENT PRODUCERS onto its own EVENT STREAM. After
doing so, it should CLEAR the EVENT STREAM that has just
been APPENDED.

There should be an UPDATE method in which the MANAGER
processes known EVENTS once PER FRAME.
*/
#endif

#include <core/Core.h>
#include <event/Stream.h>
#include <pattern/Manager.h>

namespace Nous {
namespace Event {

class Manager :
	virtual public Event::Stream,
	virtual protected Pattern::Manager<Event::Stream*>
{
public:
	typedef Pattern::Manager<Event::Stream*> BaseManager;
	typedef BaseManager::ItemList Stream_l;

public:
	Manager();
	virtual ~Manager();

public:
	inline virtual bool addEventProducer(Event::Stream *stream);
	inline virtual bool removeEventProducer(Event::Stream *stream);
	inline virtual void clearEventProducers();
	virtual bool processEventStream() = 0; /// @note Usually called once per frame.

protected:
	virtual uint32_t aggregate();
}; // class Manager

} // namespace Event
} // namespace Nous

#endif // NOUS_EVENT_MANAGER_H
