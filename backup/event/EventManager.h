///
/// @file EventManager.h
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

#ifndef RSSD_EVENTMANAGER_H
#define RSSD_EVENTMANAGER_H

#include "System"
#include "Pattern"
#include "Event.h"

namespace RSSD {

///
/// @plan EVENTS should be simple, lightweight structures
///   that are capable of being serialized to different
///   target formats, such as binary or XML.
///
///   Anyone may subscribe to event notifications for
///   particular events with the EVENT MANAGER. SUBSCRIBERS
///   may be local or remote, which will determine how the
///   EVENT MANAGER distributes event NOTIFICATIONS, i.e.
///   either locally to a SUBSCRIBER object or remotely
///   via the NETWORK MANAGER.
///
///   Anyone may submit an EVENT to the EVENT MANAGER.
///   The *one distinction* is whether an EVENT is submitted
///   by some local SOURCE or by a remote SOURCE via the
///   NETWORK MANAGER. This knowledge is required to short-
///   circuit the distribution of remote EVENTS, that is
///   ensure that they are not sent back out to remote
///   SUBSCRIBERS because the original sender should have
///   that covered.
///
///   The EVENT SYSTEM will employ the PUBLISHER/SUBSCRIBER
///   design pattern. The EVENT MANAGER is the PUBLISHER.
///
class EventManager :
  public Pattern::Singleton<EventManager>,
  public Pattern::Publisher<Event*>,
  protected Pattern::Manager<Event*>
{
public:
  typedef Pattern::Manager<Event*> Manager;
  typedef Pattern::Publisher<Event*> Publisher;

  EventManager();
  virtual ~EventManager();
  virtual uint32_t getNumEvents() const;
  virtual bool addEvent(Event *event);
  virtual bool removeEvent(Event *event);
  virtual void clearEvents();
}; /// class EventManager

} /// namespace RSSD

#endif // RSSD_EVENTMANAGER_H
