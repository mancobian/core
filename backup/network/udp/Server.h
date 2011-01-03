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

#ifndef NOUS_NETWORK_UDP_SERVER_HPP
#define NOUS_NETWORK_UDP_SERVER_HPP

#include <network/Common.h>
#include <network/Connection.h>
#include <network/Packet.h>
#include <network/Server.h>
#include <network/udp/Connection.h>

namespace RSSD {
namespace Network {
namespace UDP {

class Server : public RSSD::Network::Server
{
public:
	typedef boost::shared_ptr<Server> Pointer;

public:
	Server(
		io_service_ptr service,
		const uint16_t listen_port = 0,
		const uint16_t remote_port = 0,
		const string_t &log_group = RSSD::Network::UDP::Server::getBuiltinLogGroup());
	virtual ~Server();

public:
	inline UDP::Server::Pointer getPointer() { return boost::dynamic_pointer_cast<UDP::Server>(this->shared_from_this()); }
	virtual inline uint16_t getListenPort() const { return this->_connection->getListenPort(); }
	virtual inline void setListenPort(const uint16_t value) { this->_connection->setListenPort(value); }
	virtual inline const uint16_t getBroadcastPort() const { return this->_connection->getBroadcastPort(); }
	virtual inline void setBroadcastPort(const uint16_t value) { return this->_connection->setBroadcastPort(value); }

public:
	virtual bool start();
	virtual bool stop();
	virtual bool broadcast(const Packet &packet);

protected:
	static const char* getBuiltinLogGroup() { return "NET/UDP/SERVER"; }

protected:
	virtual void onStart();
	virtual void onStop();

protected:
	virtual void onConnectionOpened(RSSD::Network::Connection::Pointer connection);
	virtual void onConnectionClosed(RSSD::Network::Connection::Pointer connection);
	virtual void onPacketReceived(Connection::Pointer connection, const Packet &packet);
	virtual void onPacketSent(Connection::Pointer connection, const Packet &packet);

protected:
	UDP::Connection::Pointer _connection;
}; // class Server

} // namespace UDP
} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORK_UDP_SERVER_HPP
