#include "Message.h"

using namespace RSSD;
using namespace RSSD::Network;

std::ostream& RSSD::Network::operator <<(
	std::ostream &stream,
	const Message &message)
{
	return message.print(stream);
}

template<>
Message::FactoryManager*
RSSD::Pattern::Singleton<Message::FactoryManager>::_instance = NULL;

Message::Header Message::Header::parse(const byte *bytes)
{
	Message::Header header;
	Buffer buffer(bytes, Message::Header::getSize());
	buffer.rpos(0);
	buffer >> header.Type;
	buffer >> header.Version;
	buffer >> header.Size;
	return header;
}

Message::Message(const uint32_t version)
{
	this->setBuffer(NULL);
	this->setVersion(version);
}

Message::Message(
	const byte *buffer,
	const uint32_t version)
{
	this->setBuffer(buffer);
	this->setVersion(version);
}

Message::Message(const Message &rhs)
{
	this->_buffer = rhs._buffer;
}

Message::~Message()
{
}

Message& Message::operator =(const Message &rhs)
{
	if (this != &rhs)
		this->_buffer = rhs._buffer;
	return *this;
}

bool Message::operator ==(const Message &rhs) const
{
	return (this->_buffer == rhs._buffer);
}

uint16_t Message::getType() const
{
	uint16_t value = 0;
	return this->getHeaderMemberValue(TYPE_OFFSET, value);
}

void Message::setType(const uint16_t value)
{
	this->setHeaderMemberValue(TYPE_OFFSET, value);
}

uint16_t Message::getVersion() const
{
	uint16_t value = 0;
	return this->getHeaderMemberValue(VERSION_OFFSET, value);
}

void Message::setVersion(const uint16_t value)
{
	this->setHeaderMemberValue(VERSION_OFFSET, value);
}

uint32_t Message::getPayloadSize() const
{
	uint32_t value = 0;
	return this->getHeaderMemberValue(SIZE_OFFSET, value);
}

void Message::setPayloadSize(const uint32_t value)
{
	this->setHeaderMemberValue(SIZE_OFFSET, value);
}

uint32_t Message::getSize() const
{
	// return this->getPayloadSize() + Message::Header::getSize();
	return this->_buffer.size();
}

const byte* Message::getBuffer()
{
	this->setPayloadSize(this->_buffer.size() - Message::Header::getSize());
	return this->_buffer.data();
}

const byte* Message::getBuffer() const
{
	return this->_buffer.data();
}

void Message::setBuffer(const byte *buffer)
{
	Message::Header header = Message::Header::parse(buffer);
	this->_buffer.wpos(0);
	this->_buffer.write(
		buffer,
		Message::Header::getSize() + header.Size);
	this->setPayloadSize(header.Size);
}

Message::Header Message::getHeader() const
{
	Header header = {
		this->getType(),
		this->getVersion(),
		this->getPayloadSize()
	};
	return header;
}

void Message::setHeader(const Message::Header &header)
{
	this->setHeader(
		header.Type,
		header.Version,
		header.Size);
}

void Message::setHeader(
	const uint16_t type,
	const uint16_t version,
	const uint32_t size)
{
	this->setType(type);
	this->setVersion(version);
	this->setPayloadSize(size);
	this->_buffer.wpos(DATA_OFFSET);
}

void Message::clear()
{
	this->_buffer.erase(DATA_OFFSET);
}

std::ostream& Message::print(std::ostream &stream) const
{
	stream << "Message::";
	this->_buffer.print(stream);
	return stream;
}

const byte* Message::toBytes()
{
	return this->getBuffer();
}

void Message::fromBytes(const byte *buffer)
{
	this->setBuffer(buffer);
}
