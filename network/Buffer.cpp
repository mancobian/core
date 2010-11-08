#include "Buffer.h"

using namespace RSSD;
using namespace RSSD::Network;

std::ostream& RSSD::Network::operator <<(
	std::ostream &stream,
	const Buffer &buffer)
{
	return buffer.print(stream);
}

template <>
Buffer& RSSD::Network::operator << <std::string>(Buffer &buffer, std::string &value)
{
	const uint32_t length = value.size();
	buffer.write(
		reinterpret_cast<const byte*>(&length),
		sizeof(length));
	buffer.write(
		reinterpret_cast<const RSSD::byte*>(value.c_str()),
		length);
	// NLOG ("Writing element to buffer with value: " << value << " (" << length << " bytes @ wpos:" << buffer.wpos() << ")", "BUFFER");
	return buffer;
}

template <>
Buffer& RSSD::Network::operator >> <std::string>(Buffer &buffer, std::string &value)
{
	uint32_t length = 0;
	buffer.read(reinterpret_cast<byte*>(&length), sizeof(length));

	byte bytes[length];
	buffer.read(bytes, length);
	value.assign(reinterpret_cast<const char*>(bytes), length);
	// NLOG ("Reading element from buffer with value: " << value << " (" << length << " bytes @ rpos:" << buffer.rpos() << ")", "BUFFER");
	return buffer;
}

template <>
Buffer& RSSD::Network::operator << <Buffer>(Buffer &lhs, Buffer &rhs)
{
	lhs = rhs;
	// NLOG ("Writing element to buffer with value: (" << lhs.size() << " bytes @ write-pos:" << lhs.wpos() << "): " << lhs, "BUFFER");
	return lhs;
}

template <>
Buffer& RSSD::Network::operator >> <Buffer>(Buffer &lhs, Buffer &rhs)
{
	rhs = lhs;
	// NLOG ("Reading element from buffer (" << rhs.size() << " bytes @ read-pos:" << lhs.rpos() << "): " << rhs, "BUFFER");
	return lhs;
}

Buffer& RSSD::Network::rpos(
	Buffer &buffer,
	const uint32_t value)
{
	buffer.rpos(value);
	return buffer;
}

Buffer& RSSD::Network::wpos(
	Buffer &buffer,
	const uint32_t value)
{
	buffer.wpos(value);
	return buffer;
}

Buffer::Buffer() :
	_rpos(0),
	_wpos(0)
{
}

Buffer::Buffer(
	const byte *value,
	const uint32_t size) :
	_rpos(0),
	_wpos(0)
{
	this->write(value, size);
}

Buffer::Buffer(const Buffer &rhs) :
	_rpos(0),
	_wpos(0)
{
	this->_data.assign(
		rhs._data.begin(),
		rhs._data.end());
}

Buffer::~Buffer()
{
}

Buffer& Buffer::operator =(const Buffer &rhs)
{
	this->clear();
	if (this != &rhs)
	{
		this->_data.assign(
			rhs._data.begin(),
			rhs._data.end());
	}
	return *this;
}

bool Buffer::operator ==(const Buffer &rhs) const
{
	return std::equal(
		this->_data.begin(),
		this->_data.end(),
		rhs._data.begin());
}

uint32_t Buffer::rpos() const
{
	return this->_rpos;
}

uint32_t Buffer::rpos(const uint32_t value)
{
	uint32_t oldpos = this->_rpos;
	this->_rpos = value;
	return oldpos;
}

uint32_t Buffer::wpos() const
{
	return this->_wpos;
}

uint32_t Buffer::wpos(const uint32_t value)
{
	uint32_t oldpos = this->_wpos;
	this->_wpos = value;
	return oldpos;
}

uint32_t Buffer::size() const
{
	return this->_data.size();
}

bool Buffer::empty() const
{
	return this->_data.empty();
}

void Buffer::resize(const uint32_t size)
{
	this->_data.resize(size);

	if (this->_rpos > this->_data.size())
		this->_rpos = this->_data.size();
	if (this->_wpos > this->_data.size())
		this->_wpos = this->_data.size();
}

void Buffer::clear()
{
	this->_data.clear();
	this->_rpos = this->_wpos = 0;
}

void Buffer::erase(
	const uint32_t offset,
	const uint32_t length)
{
	/// @validate Erase start index must not be
	/// greater than the length of the entire buffer.
	if (offset > this->_data.size())
		throw std::out_of_range("Buffer::erase");

	byte_v::iterator begin = this->_data.begin() + offset;
	byte_v::iterator end = this->_data.end();
	if (((offset + length) < this->_data.size()))
		end = begin + length;

	this->_data.erase(begin, end);

	if (this->_rpos > this->_data.size())
		this->_rpos = this->_data.size();
	if (this->_wpos > this->_data.size())
		this->_wpos = this->_data.size();
}

uint32_t Buffer::insert(
	const byte *value,
	const uint32_t length,
	const uint32_t offset)
{
	if (!value || !length)
		return 0;
	else if (offset > this->_data.size())
		throw std::out_of_range("Buffer::insert");

	/// @note Notice that the range includes all the
	/// elements between first and last, including the
	/// first element but not the last element.
	this->_data.insert(
		this->_data.begin() + offset,
		&value[0],
		&value[length]);
	this->_wpos = offset + length;
	return length;
}

Buffer& Buffer::setpos(const Mode mode, const uint32_t index)
{
	if (mode & READ)
		this->rpos(index);
	if (mode & WRITE)
		this->wpos(index);
	return *this;
}

const byte* Buffer::data(const uint32_t offset) const
{
	if (this->_data.empty())
		return NULL;
	else if (offset > this->_data.size())
		throw std::out_of_range("Buffer::data");
	return &this->_data[offset];
}

uint32_t Buffer::read(
	byte* bytes,
	const uint32_t size)
{
	if (this->_data.empty()
		|| !size
		|| !bytes)
		return 0;
	else if (this->_data.size() < (this->_rpos + size))
		throw std::out_of_range("Buffer::read");

	std::memcpy(
		bytes,
		&this->_data[this->_rpos],
		size);
	this->_rpos += size;
	return size;
}

uint32_t Buffer::read(
	byte* bytes,
	const uint32_t size,
	const uint32_t offset) const
{
	if (this->_data.empty()
		|| !size
		|| !bytes)
		return 0;
	else if (this->_data.size() < (offset + size))
		throw std::out_of_range("Buffer::read");

	std::memcpy(
		bytes,
		&this->_data[offset],
		size);
	return size;
}

uint32_t Buffer::write(
	const byte *value,
	const uint32_t size,
	const uint32_t offset)
{
	if (!value || !size)
	{
        NLOG ("Invalid data or size.", "NET/BUFFER", Log::Log::Level::ERROR);
		return 0;
	}
	else if (this->_data.size() < (this->_wpos + size))
	{
		this->resize(this->_wpos + size);
		// throw std::out_of_range("Buffer::write");
		// NLOG ("Resizing buffer: " << this->_data.size() << " bytes", "NET/BUFFER", Log::Level::INSANE);
	}

	if (offset != UINT_MAX)
	{
		this->_wpos = offset;
		// NLOG ("Set buffer write pos: " << this->_wpos, "NET/BUFFER", Log::Level::INSANE);
	}

	std::memcpy(
		&this->_data[this->_wpos],
		value,
		size);
	this->_wpos += size;
	// NLOG ("Copied " << size << " bytes into buffer.", "NET/BUFFER", Log::Level::INSANE);
	// NLOG ("Set buffer write pos: " << this->_wpos, "NET/BUFFER", Log::Level::INSANE);
	return size;
}

std::ostream& Buffer::print(std::ostream &stream) const
{
	stream << "Buffer=" << this->_data;
	return stream;
}
