///
/// @file BasicPacket.h
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

#ifndef NOUS_NETWORK_BASICPACKET_H
#define NOUS_NETWORK_BASICPACKET_H

#include <network/Common.h>
#include <network/Message.h>
#include <network/Packet.h>

namespace RSSD {
namespace Network {

class BasicPacket : virtual public Packet
{
public:
	static const uint32_t TYPE = 1;

public:
	BasicPacket() :
		Packet(TYPE)
	{
	}

	BasicPacket(const BasicPacket &rhs) :
		Packet(rhs)
	{}

	BasicPacket(const Packet &rhs) :
		Packet(rhs)
	{}

	BasicPacket(
		const byte *payload,
		const uint32_t size) :
		Packet(TYPE, payload, size)
	{}

	BasicPacket(const Message *message) :
		Packet(TYPE, message)
	{}

	virtual ~BasicPacket()
	{}

	Message::Pointer getMessage()
	{
		const byte* bytes = this->getPayload();
		Message::Header header = Message::Header::parse(bytes);
		Message::Pointer message(
			Message::FactoryManager::getPointer()->create(header.Type));
		message->setBuffer(bytes);
		return message;
	}

	void setMessage(const Message &message)
	{
		Message &mutable_message = const_cast<Message&>(message);
		const byte* bytes = mutable_message.toBytes();
		this->setPayload(
			bytes,
			mutable_message.getSize());
	}
}; // class BasicPacket

} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORK_BASICPACKET_H
