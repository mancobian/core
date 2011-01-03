///
/// @file Stream.h
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

#ifndef NOUS_EVENT_STREAM_H
#define NOUS_EVENT_STREAM_H

#include <core/Core.h>
#include <event/Packet.h>

namespace Nous {
namespace Event {

class Stream
{
public:
	class Packet : public Event::Packet
	{
	public:
		Packet(const byte *data = NULL);
		Packet(const byte_v &data);
		Packet(const Packet &rhs);
		~Packet();

	public:
		inline virtual uint32_t getType() const { return static_cast<uint32_t>(this->_data[0]); }
		inline virtual uint32_t getSize() const { return static_cast<uint32_t>(this->_data[sizeof(uint32_t)]); }
		inline virtual uint64_t getTime() const { return static_cast<uint64_t>(this->_data[sizeof(uint32_t) << 1]); }
		inline void setData(const byte *value) { this->_data = value; }
		inline const byte* getData() const { return this->_data; }

	public:
		Packet& operator =(const Packet &rhs);

	public:
		void clear();

	protected:
		const byte *_data;
	}; // class Packet

public:
	Stream();
	Stream(const Stream &rhs);
	Stream(const byte *data);
	virtual ~Stream();

public:
	inline uint32_t getSize() const { return this->_data.size(); }
	inline uint32_t getOffset() const { return this->_offset; }
	inline uint32_t getNumConsumers() const { return this->_num_consumers; }
	inline uint32_t getNumReads() const { return this->_num_reads; }
	inline void setOffset(const uint32_t value) { this->_offset = value; }

public:
	Stream& operator =(const Stream &rhs);

public:
	bool append(const byte *data);
	bool append(const byte_v &data);
	bool append(const Event::Packet *event);
	bool append(const Stream *other);
	bool peek(Packet *packet);
	bool read(Packet *packet);
	void reset();
	void clear();
	bool flush();

protected:
	void incrementNumConsumers() { ++this->_num_consumers; }
	void decrementNumConsumers() { --this->_num_consumers; }
	void incrementNumReads() { ++this->_num_reads; }
	void decrementNumReads() { --this->_num_reads; }
	bool append(Stream *other);
	bool append(const Packet &packet);

protected:
	uint32_t _offset,
		_num_reads,
		_num_consumers;
	byte_v _data;

private:
	friend class Manager;
}; // class Stream

} // namespace Event
} // namespace Nous

#endif // NOUS_EVENT_STREAM_H
