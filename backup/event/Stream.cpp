#include <event/Stream.h>

using namespace Nous;
using namespace Nous::Event;

///
/// Stream::Packet
///

Stream::Packet::Packet(const byte *data) :
	_data(data)
{
}

Stream::Packet::Packet(const byte_v &data) :
	_data(&data[0])
{
}

Stream::Packet::Packet(const Packet &rhs)
{
	this->_data = rhs._data;
}

Stream::Packet::~Packet()
{
	this->clear();
}

Stream::Packet& Stream::Packet::operator =(
	const Stream::Packet &rhs)
{
	if (this == &rhs)
		return *this;
	this->_data = rhs._data;
	return *this;
}

void Stream::Packet::clear()
{
	this->_data = NULL;
}

///
/// Stream
///

Stream::Stream() :
	_offset(0),
	_num_reads(0),
	_num_consumers(0)
{}

Stream::Stream(const Stream &rhs)
{
	this->_offset = rhs._offset;
	this->_num_reads = rhs._num_reads;
	this->_num_consumers = rhs._num_consumers;
	this->_data.assign(rhs._data.begin(), rhs._data.end());
}

Stream::Stream(const byte *data) :
	_offset(0),
	_num_reads(0),
	_num_consumers(0)
{
	this->append(data);
}

Stream::~Stream()
{
	this->clear();
}

Stream& Stream::operator =(const Stream &rhs)
{
	if (this == &rhs)
		return *this;
	this->_offset = rhs._offset;
	this->_data.assign(rhs._data.begin(), rhs._data.end());
	return *this;
}

///
/// @note Method expects header + payload byte array.
///

bool Stream::append(const byte *data)
{
	Stream::Packet packet(data);
	return this->append(packet);
}

bool Stream::append(const byte_v &data)
{
	Stream::Packet packet(data);
	return this->append(packet);
}

bool Stream::append(const Event::Packet *event)
{
	byte_v data = event->serialize();
	return this->append(data);
}

bool Stream::append(const Stream *other)
{
	if (other->_data.empty())
		return false;

	this->_data.insert(
		this->_data.end(),
		other->_data.begin(),
		other->_data.end());
	return true;
}

bool Stream::append(Stream *other)
{
	if (!this->append(const_cast<const Stream*>(other)))
		return false;
	this->incrementNumReads();
	return true;
}

bool Stream::append(const Packet &packet)
{
	this->_data.insert(
		this->_data.end(),
		packet.getData()[0],
		packet.getData()[packet.getSize()]);
	return true;
}

bool Stream::peek(Stream::Packet *packet)
{
	if (!packet)
		return false;
	packet->setData(&this->_data[this->_offset]);
	return true;
}

bool Stream::read(Stream::Packet *packet)
{
	if (!packet || (this->_offset >= this->_data.size()))
		return false;
	packet->setData(&this->_data[this->_offset]);
	this->_offset += packet->getHeaderSize() + packet->getSize();
	return true;
}

void Stream::reset()
{
	this->_offset = 0;
}

void Stream::clear()
{
	this->_offset = 0;
	this->_num_reads = 0;
	this->_num_consumers = 0;
	this->_data.clear();
}

bool Stream::flush()
{
	if (this->getNumReads() < this->getNumConsumers())
		return false;
	this->clear();
	return true;
}
