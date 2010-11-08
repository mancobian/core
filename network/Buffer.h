///
/// @file Buffer.h
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

#ifndef NOUS_NETWORK_BUFFER_H
#define NOUS_NETWORK_BUFFER_H

#include <stdexcept>
#include <iomanip>
#include <network/Common.h>

namespace RSSD {
namespace Network {

class Buffer
{
public:
	enum Mode
	{
		READ = 1<<0,
		WRITE = 1<<1
	}; // enum Mode

public:
	Buffer();
	Buffer(
		const byte *value,
		const uint32_t size);
	Buffer(const Buffer &rhs);
	virtual ~Buffer();

public:
	///
	/// @todo Add stream manipulators for
	/// std collections of POD types.
	///

	Buffer& operator =(const Buffer &rhs);
	bool operator ==(const Buffer &rhs) const;
	friend std::ostream& operator <<(
		std::ostream &stream,
		const Buffer &buffer);
	template <typename T> friend Buffer& operator <<(
		Buffer &buffer,
		T &value);
	template <typename T> friend Buffer& operator >>(
		Buffer &buffer,
		T &value);
	friend Buffer& rpos(Buffer &buffer, const uint32_t value);
	friend Buffer& wpos(Buffer &buffer, const uint32_t value);

public:
	bool empty() const;
	uint32_t size() const;
	void erase(
		const uint32_t offset = 0,
		const uint32_t length = 0);
	void clear(); /// @note Alias for Buffer::erase()
	uint32_t insert(
		const byte *value,
		const uint32_t length,
		const uint32_t offset = 0);
	void resize(const uint32_t length);
	const byte* data(const uint32_t offset = 0) const;
	uint32_t read(
		byte* bytes,
		const uint32_t length);
	uint32_t read(
		byte* bytes,
		const uint32_t length,
		const uint32_t offset = 0) const;
	uint32_t write(
		const byte *bytes,
		const uint32_t length,
		const uint32_t offset = UINT_MAX);
	Buffer& setpos(const Mode mode, const uint32_t index);
	uint32_t rpos() const;
	uint32_t wpos() const;
	uint32_t rpos(const uint32_t value);
	uint32_t wpos(const uint32_t value);
	std::ostream& print(std::ostream &out = std::cout) const;

protected:
	uint32_t _rpos,
		_wpos;
	byte_v _data;
}; // class Buffer

///
/// Global Functions
///

std::ostream& operator <<(
	std::ostream &stream,
	const Buffer &buffer);

template <typename T>
Buffer& operator <<(Buffer &buffer, T &value)
{
	const uint32_t size = sizeof(value);
	// NLOG ("(01) Writing element to buffer with value: " << value << " (" << size << " bytes @ wpos:" << buffer.wpos() << ")", "BUFFER");
	buffer.write(reinterpret_cast<const byte*>(&value), size);
	// NLOG ("(02) Writing element to buffer with value: " << value << " (" << size << " bytes @ wpos:" << buffer.wpos() << ")", "BUFFER");
	return buffer;
}

template <typename T>
Buffer& operator >>(Buffer &buffer, T &value)
{
	// NLOG ("(01) Reading element from buffer with value: " << value << " (" << size << " bytes @ rpos:" << buffer.rpos() << ")", "BUFFER");
	buffer.read(reinterpret_cast<byte*>(&value), sizeof(value));
	// NLOG ("(02) Reading element from buffer with value: " << value << " (" << size << " bytes @ rpos:" << buffer.rpos() << ")", "BUFFER");
	return buffer;
}

template <> Buffer& operator << <std::string>(Buffer &buffer, std::string &value);
template <> Buffer& operator >> <std::string>(Buffer &buffer, std::string &value);
template <> Buffer& operator << <Buffer>(Buffer &buffer, Buffer &value);
template <> Buffer& operator >> <Buffer>(Buffer &buffer, Buffer &value);

Buffer& rpos(Buffer &buffer, const uint32_t value);
Buffer& wpos(Buffer &buffer, const uint32_t value);

} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORK_BUFFER_H
