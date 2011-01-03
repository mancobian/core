///
/// @file Packet.h
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

#ifndef NOUS_NETWORK_PACKET_H
#define NOUS_NETWORK_PACKET_H

#include <network/Common.h>
#include <network/Buffer.h>
#include <network/Message.h>

namespace RSSD {
namespace Network {

///
/// @class Packet
///

class Packet
{
public:
	struct Header
	{
	public:
		uint32_t Type;
		uint32_t Size;

	public:
		static uint32_t getSize();
		static Header parse(const byte *bytes);
	}; // struct Header

public:
	static const uint32_t TYPE_OFFSET = 0;
	static const uint32_t SIZE_OFFSET = TYPE_OFFSET + sizeof(TYPE_OFFSET);
	static const uint32_t DATA_OFFSET = SIZE_OFFSET + sizeof(SIZE_OFFSET);
	static const uint32_t MAX_UDP_SIZE = 1500; /// @note Arbitrarily chosen size based on (UDP?) MTU of 1500 bytes.
	static const char* NLOG_GROUP_ID;

public:
	Packet();
	Packet(const byte *buffer);
	Packet(
		const uint32_t type,
		const byte *payload,
		const uint32_t size);
	Packet(
		const uint32_t type,
		const Message *message = NULL);
	Packet(const Packet &rhs);
	virtual ~Packet();

public:
	virtual Packet& operator =(const Packet &rhs);
	virtual bool operator ==(const Packet &rhs) const;

public:
	virtual uint32_t getType() const;
	virtual void setType(const uint32_t value);
	virtual uint32_t getPayloadSize() const;
	virtual void setPayloadSize(const uint32_t value);
	virtual uint32_t getSize() const;
	virtual const byte* getPayload() const;
	virtual void setPayload(
		const byte *buffer,
		const uint32_t size);
	virtual Header getHeader();
	virtual void setHeader(
		const uint32_t type,
		const uint32_t size = 0);

public:
	virtual void clear(const uint32_t reserve = 0);
	virtual void resize(const uint32_t size);
	virtual const byte* toBytes();
	virtual void fromBytes(const byte* data);
	virtual std::ostream& print(std::ostream &stream = std::cout) const;

protected:
	virtual uint32_t getHeaderMemberValue(const uint32_t index) const;
	virtual void setHeaderMemberValue(
		const uint32_t index,
		const uint32_t value);

protected:
	Buffer _buffer; /// @note Header + Payload
}; // class Packet

///
/// Typedefs
///

TYPEDEF_CONTAINERS(Packet);

///
/// @class PacketQueue
///

class PacketQueue
{
public:
	PacketQueue();
	virtual ~PacketQueue();

public:
	bool empty() const;
	uint32_t size() const;
	void push(const Packet &packet);
	void pop();
	void pop(Packet &packet);
	Packet& front();
	const Packet& front() const;
	void clear();

public:
	mutable boost::mutex _mutex;
	Packet_q _packets;
}; // class PacketQueue

///
/// Stream Manipulation Operators
///

std::ostream& operator <<(
	std::ostream &stream,
	const Packet &packet);

} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORK_PACKET_H
