///
/// @file Connection.h
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

#ifndef NOUS_NETWORK_TCP_CONNECTION_HPP
#define NOUS_NETWORK_TCP_CONNECTION_HPP

#include <network/Common.h>
#include <network/Packet.h>
#include <network/InternetConnection.h>

namespace RSSD {
namespace Network {
namespace TCP {

class Connection :
	private boost::noncopyable,
	public InternetConnection<boost::asio::ip::tcp>
{
public:
	typedef boost::shared_ptr<Connection> Pointer;
	typedef InternetConnection<boost::asio::ip::tcp> BaseConnection;

public:
	Connection(io_service_ptr service);
	Connection(
		io_service_ptr service,
		const std::string &host,
		const unsigned short port);
	virtual ~Connection();

public:
	inline TCP::Connection::Pointer getPointer() { return boost::dynamic_pointer_cast<TCP::Connection>(this->shared_from_this()); }

public:
	virtual bool send(const Packet &packet);
	virtual bool receive();

protected:
	void onSend(const Packet &packet);
	void onWrite(
		const Packet &packet,
		const uint32_t bytes_transferred,
		const boost::system::error_code &error);
	void onReceive();
	void onReceiveHeader(
		const uint32_t bytes_transferred,
		const boost::system::error_code &error);
	void onReceivePayload(
		const uint32_t bytes_transferred,
		const boost::system::error_code &error);

protected:
	static const bnet::ip::tcp::resolver::iterator RESOLVE_END;
}; // class Connection

} // namespace TCP
} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORK_TCP_CONNECTION_HPP
