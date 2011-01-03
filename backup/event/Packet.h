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

#ifndef NOUS_EVENT_PACKET_H
#define NOUS_EVENT_PACKET_H

#include <core/Core.h>
#include <event/Type.h>

namespace Nous {
namespace Event {

class Packet
{
public:
	static uint32_t getHeaderSize()
	{
		return sizeof(uint32_t) << 2;
	}

public:
	Packet(const uint32_t type = Event::UNKNOWN) : _type(type) {}
	virtual ~Packet() {}

public:
	inline virtual uint32_t getType() const { return static_cast<uint32_t>(this->_type); }
	inline virtual uint32_t getSize() const { return this->getHeaderSize(); }
	inline virtual uint64_t getTime() const { return this->_time; }

public:
	virtual byte_v serialize() const
	{
		byte_v data(this->getSize());
		data.insert(data.end(),
			((char*)&this->_type)[0],
			((char*)&this->_type)[sizeof(this->_type)]);
		data.insert(data.end(),
			((char*)&this->_size)[0],
			((char*)&this->_size)[sizeof(this->_size)]);
		data.insert(data.end(),
			((char*)&this->_time)[0],
			((char*)&this->_time)[sizeof(this->_time)]);
		return data;
	}

protected:
	uint32_t _type;
	uint32_t _size;
	uint64_t _time;
}; // class Packet

} // namespace Network
} // namespace Nous

#endif // NOUS_EVENT_PACKET_H
