///
/// @file Message.h
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

#ifndef NOUS_NETWORK_MESSAGE_H
#define NOUS_NETWORK_MESSAGE_H

#include "Pattern"
#include <network/Common.h>
#include <network/Buffer.h>

namespace RSSD {
namespace Network {
class Message;
} // namespace Network
} // namespace RSSD

namespace RSSD {
namespace Network {

class Message
{
public:
	typedef boost::shared_ptr<Message> Pointer;
	typedef RSSD::Core::Pattern::FactoryManager<Message> FactoryManager;
	typedef RSSD::Core::Pattern::Factory<Message> Factory;

public:
	struct Header
	{
	public:
		uint16_t Type, Version;
		uint32_t Size;

	public:
		static uint32_t getSize() { return sizeof(uint32_t) * 2; }
		static Header parse(const byte *bytes);
	}; // struct Header

public:
	Message(const uint32_t version);
	Message(
		const byte *buffer,
		const uint32_t version);
	Message(const Message &rhs);
	virtual ~Message();

public:
	static const uint16_t TYPE_OFFSET = 0;
	static const uint16_t VERSION_OFFSET = TYPE_OFFSET + sizeof(TYPE_OFFSET);
	static const uint32_t SIZE_OFFSET = VERSION_OFFSET + sizeof(VERSION_OFFSET);
	static const uint32_t DATA_OFFSET = SIZE_OFFSET + sizeof(SIZE_OFFSET);

public:
	virtual Message& operator =(const Message &rhs);
	virtual bool operator ==(const Message &rhs) const;
	friend std::ostream& operator <<(
		std::ostream &stream,
		const Message &message);

public:
	virtual uint16_t getType() const;
	virtual void setType(const uint16_t value);
	virtual uint16_t getVersion() const;
	virtual void setVersion(const uint16_t value);
	virtual uint32_t getPayloadSize() const;
	virtual void setPayloadSize(const uint32_t value);
	virtual uint32_t getSize() const;
	virtual const byte* getBuffer() const;
	virtual void setBuffer(const byte *buffer);
	virtual Header getHeader() const;
	virtual void setHeader(const Header &header);
	virtual void setHeader(
		const uint16_t type,
		const uint16_t version,
		const uint32_t size = 0);

public:
	virtual void clear();
	virtual std::ostream& print(std::ostream &stream = std::cout) const;
	virtual const byte* toBytes() = 0;
	virtual void fromBytes(const byte *buffer = NULL) = 0;

protected:
	virtual const byte* getBuffer();
	template <typename T> T& getHeaderMemberValue(
		const uint32_t index,
		T &value) const;
	template <typename T> void setHeaderMemberValue(
		const uint32_t index,
		const T value);

protected:
	Buffer _buffer; /// @note Header + Payload
}; // class Message

template <typename T>
class BaseMessage : public Message
{
public:
	class Factory :
		public RSSD::Core::Pattern::Singleton<typename BaseMessage<T>::Factory>,
		virtual public Message::Factory
	{
	public:
		typedef RSSD::Core::Pattern::Singleton<typename BaseMessage<T>::Factory> Singleton;

	public:
		Factory() : Message::Factory()
		{
			NLOG ("Created basic message factory with ID <" << this->_type << ">", "NET/MESSAGE/FACTORY");
		}

	protected:
		virtual T* createImpl() const { return new T(); }
	}; // class Factory

public:
	BaseMessage(const uint32_t version = 1) :
		Message(version)
	{
		this->setType(Factory::getPointer()->getType());
	}

	BaseMessage(
		const byte *buffer,
		const uint32_t version = 1) :
		Message(buffer, version)
	{
		this->setType(Factory::getPointer()->getType());
	}

	virtual ~BaseMessage()
	{}
}; // class BaseMessage

///
/// Stream Manipulation Operators
///

std::ostream& operator <<(
	std::ostream &stream,
	const Message &message);

///
/// Template Function Definitions
///

template <typename T>
T& Message::getHeaderMemberValue(
	const uint32_t index,
	T &value) const
{
	if (this->_buffer.empty())
		return value;
	this->_buffer.read(
		reinterpret_cast<byte*>(&value),
		sizeof(value),
		index);
	return value;
}

template <typename T>
void Message::setHeaderMemberValue(
	const uint32_t index,
	const T value)
{
	if (this->_buffer.empty())
		this->_buffer.resize(DATA_OFFSET);
	uint32_t pos = this->_buffer.wpos(index);
	this->_buffer << value;
	this->_buffer.wpos(pos);
}

} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORK_MESSAGE_H
