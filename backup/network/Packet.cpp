#include "Packet.h"

using namespace RSSD;
using namespace RSSD::Network;

///
/// @class Packet
///

const char* RSSD::Network::Packet::NLOG_GROUP_ID = "NET/PACKET";

std::ostream& RSSD::Network::operator <<(
	std::ostream &stream,
	const Packet &packet)
{
	return packet.print(stream);
}

uint32_t Packet::Header::getSize()
{
	return sizeof(uint32_t) * 2;
}

Packet::Header Packet::Header::parse(const byte *bytes)
{
	Packet::Header header;
	Buffer buffer(bytes, Packet::Header::getSize());
	buffer.rpos(0);
	buffer >> header.Type;
	buffer >> header.Size;
	return header;
}

Packet::Packet()
{
}

Packet::Packet(const byte *buffer)
{
	this->resize(Packet::Header::getSize());
	if (buffer)
		this->fromBytes(buffer);
}

Packet::Packet(
	const uint32_t type,
	const byte *payload,
	const uint32_t size)
{
	this->resize(Packet::Header::getSize());
	this->setType(type);
	if (payload)
		this->setPayload(payload, size);
}

Packet::Packet(
	const uint32_t type,
	const Message *message)
{
	this->resize(Packet::Header::getSize());
	this->setType(type);

	if (message)
	{
		// NLOG ("Packet::Ctor(): " << *message, "PACKET");
		const byte* payload = message->getBuffer();
		const uint32_t size = message->getSize();
		if (payload && size)
			this->setPayload(payload, size);
	}
}

Packet::Packet(const Packet &rhs) :
	_buffer(rhs._buffer)
{
}

Packet::~Packet()
{
	// this->_buffer.clear();
}

Packet& Packet::operator =(const Packet &rhs)
{
	if (this != &rhs)
		this->_buffer = rhs._buffer;
	return *this;
}

bool Packet::operator ==(const Packet &rhs) const
{
	return (this->_buffer == rhs._buffer);
}

uint32_t Packet::getType() const
{
	return this->getHeaderMemberValue(TYPE_OFFSET);
}

void Packet::setType(const uint32_t value)
{
	this->setHeaderMemberValue(TYPE_OFFSET, value);
}

uint32_t Packet::getPayloadSize() const
{
	assert (this->_buffer.size() == (this->getHeaderMemberValue(SIZE_OFFSET) + Packet::Header::getSize()));
	return this->getHeaderMemberValue(SIZE_OFFSET);
}

void Packet::setPayloadSize(const uint32_t value)
{
	this->setHeaderMemberValue(SIZE_OFFSET, value);
}

uint32_t Packet::getSize() const
{
	// assert ((Packet::Header::getSize() + this->getPayloadSize()) == this->_buffer.size());
	return this->_buffer.size();
}

const byte* Packet::getPayload() const
{
	if (this->_buffer.size() < DATA_OFFSET)
		return NULL;
	return this->_buffer.data() + DATA_OFFSET;
}

void Packet::setPayload(const byte *buffer, const uint32_t size)
{
	this->resize(Packet::Header::getSize() + size);
	this->_buffer.write(
		buffer,
		size,
		DATA_OFFSET);
	this->setPayloadSize(size);
}

Packet::Header Packet::getHeader()
{
	Header header = {
		this->getType(),
		this->getSize()
	};
	return header;
}

void Packet::setHeader(
	const uint32_t type,
	const uint32_t size)
{
	this->resize(Packet::Header::getSize());
	this->setType(type);
	this->setPayloadSize(size);
}

void Packet::clear(const uint32_t reserve)
{
	if (this->_buffer.size() > DATA_OFFSET)
		this->_buffer.erase(DATA_OFFSET);
	this->resize(DATA_OFFSET + reserve);
}

void Packet::resize(const uint32_t size)
{
	this->_buffer.resize(size);
}

const byte* Packet::toBytes()
{
	return this->_buffer.data();
}

void Packet::fromBytes(const byte* data)
{
	Packet::Header header = Packet::Header::parse(data);
	uint32_t size = Packet::Header::getSize() + header.Size;
	this->resize(size);
	this->_buffer.wpos(0);
	this->_buffer.write(data, size);
	this->setPayloadSize(header.Size);
}

std::ostream& Packet::print(std::ostream &stream) const
{
	stream << "Packet::";
	stream << this->_buffer;
	return stream;
}

uint32_t Packet::getHeaderMemberValue(const uint32_t index) const
{
	uint32_t value = 0;
	if (this->_buffer.empty())
		return 0;
	this->_buffer.read(
		reinterpret_cast<byte*>(&value),
		sizeof(value),
		index);
	return value;
}

void Packet::setHeaderMemberValue(
	const uint32_t index,
	const uint32_t value)
{
	if (this->_buffer.empty())
		this->_buffer.resize(DATA_OFFSET);
	uint32_t pos = this->_buffer.wpos(index);
	this->_buffer << value;
	this->_buffer.wpos(pos);
}

///
/// @class PacketQueue
///

PacketQueue::PacketQueue()
{
}

PacketQueue::~PacketQueue()
{
}

bool PacketQueue::empty() const
{
	boost::mutex::scoped_lock lock(this->_mutex);
	return this->_packets.empty();
}

uint32_t PacketQueue::size() const
{
	boost::mutex::scoped_lock lock(this->_mutex);
	return this->_packets.size();
}

void PacketQueue::push(const Packet &packet)
{
	boost::mutex::scoped_lock lock(this->_mutex);
	this->_packets.push(packet);
}

void PacketQueue::pop()
{
	boost::mutex::scoped_lock lock(this->_mutex);
	this->_packets.pop();
}

void PacketQueue::pop(Packet &packet)
{
	boost::mutex::scoped_lock lock(this->_mutex);
	packet = this->_packets.front();
	this->_packets.pop();
}

Packet& PacketQueue::front()
{
	boost::mutex::scoped_lock lock(this->_mutex);
	return this->_packets.front();
}

const Packet& PacketQueue::front() const
{
	boost::mutex::scoped_lock lock(this->_mutex);
	return this->_packets.front();
}

void PacketQueue::clear()
{
	boost::mutex::scoped_lock lock(this->_mutex);
	while (!this->_packets.empty())
		this->_packets.pop();
}
