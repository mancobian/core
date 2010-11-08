///
/// @file Server.h
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

#ifndef NOUS_NETWORK_TCP_SERVER_HPP
#define NOUS_NETWORK_TCP_SERVER_HPP

#include <network/Common.h>
#include <network/Connection.h>
#include <network/Packet.h>
#include <network/Server.h>
#include <network/tcp/Connection.h>

namespace RSSD {
namespace Network {
namespace TCP {

class Server : public RSSD::Network::Server
{
public:
	typedef boost::shared_ptr<TCP::Server> Pointer;

public:
	Server(
		io_service_ptr service,
		const uint16_t listen_port = 0,
		const std::string &log_group = TCP::Server::getBuiltinLogGroup());
	virtual ~Server();

public:
	inline TCP::Server::Pointer getPointer() { return boost::dynamic_pointer_cast<TCP::Server>(this->shared_from_this()); }
	virtual inline uint16_t getListenPort() const { return this->_listen_endpoint.port(); }
	virtual inline void setListenPort(const uint16_t value) { this->_listen_endpoint.port(value); }

public:
	virtual bool start();
	virtual bool stop();

protected:
	static const char* getBuiltinLogGroup() { return "NET/TCP/SERVER"; }

protected:
	virtual void onStart();
	void listen(const boost::system::error_code &error = boost::system::error_code());

protected:
	virtual void onStop();
	virtual void onConnectionOpened(Connection::Pointer connection);
	virtual void onConnectionClosed(Connection::Pointer connection);
	virtual void onPacketReceived(Connection::Pointer connection, const Packet &packet);
	virtual void onPacketSent(Connection::Pointer connection, const Packet &packet);

protected:
	uint32_t _client_id;
	bnet::ip::tcp::endpoint _listen_endpoint;
	bnet::ip::tcp::acceptor _acceptor;
	TCP::Connection::Pointer _pending_connection;
}; // class Server

} // namespace TCP
} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORK_TCP_SERVER_HPP
