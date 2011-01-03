///
/// @file BasicClient.h
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

#ifndef NOUS_NETWORK_BASICCLIENT_H
#define NOUS_NETWORK_BASICCLIENT_H

#include <network/Common.h>
#include <network/tcp/Client.h>
#include <network/udp/Client.h>
#include <network/Message.h>

namespace RSSD {
namespace Network {

class BasicClient : public Connection
{
public:
	typedef boost::shared_ptr<BasicClient> Pointer;

public:
	static uint32_t APPLICATION_ID;

public:
	BasicClient();
	BasicClient(io_service_ptr service);
	virtual ~BasicClient();

public:
	inline unsigned short getTcpRemotePort() const { return this->_tcp_client->getRemotePort(); }
	inline void setTcpRemotePort(const unsigned short value) { this->_tcp_client->setRemotePort(value); }
	inline string_t getTcpRemoteAddress() { return this->_tcp_client->getRemoteAddress(); }
	inline void setTcpRemoteAddress(const string_t &value) { this->_tcp_client->setRemoteAddress(value); }

	inline string_t getUdpListenAddress() const { return this->_udp_client->getListenAddress(); }
	inline void setUdpListenAddress(const string_t &value) { this->_udp_client->setListenAddress(value); }
	inline unsigned short getUdpListenPort() const { return this->_udp_client->getListenPort(); }
	inline void setUdpListenPort(const unsigned short value) { this->_udp_client->setListenPort(value); }

	inline string_t getUdpRemoteAddress() const { return this->_udp_client->getRemoteAddress(); }
	inline void setUdpRemoteAddress(const string_t &value) { this->_udp_client->setRemoteAddress(value); }
	inline unsigned short getUdpRemotePort() const { return this->_udp_client->getRemotePort(); }
	inline void setUdpRemotePort(const unsigned short value) { this->_udp_client->setRemotePort(value); }

	inline string_t getUdpBroadcastAddress() const { return this->_udp_client->getBroadcastAddress(); }
	inline void setUdpBroadcastAddress(const string_t &value) { this->_udp_client->setBroadcastAddress(value); }
	inline unsigned short getUdpBroadcastPort() const { return this->_udp_client->getBroadcastPort(); }
	inline void setUdpBroadcastPort(const unsigned short value) { this->_udp_client->setBroadcastPort(value); }

	inline BasicClient::Pointer getPointer() { return boost::dynamic_pointer_cast<BasicClient>(this->shared_from_this()); }
	virtual inline void setId(const uint32_t value)
	{
		if (this->_tcp_client) { this->_tcp_client->setId(value); }
		if (this->_udp_client) { this->_udp_client->setId(value); }
		Connection::setId(value);
	}

public:
	virtual bool open();
	virtual bool close();
	virtual bool receive();
	virtual bool send(const Message &message, const bool is_critical = true);
	virtual bool send(const Packet &packet);
	virtual bool send(const Packet &packet, const bool is_critical);
	virtual bool broadcast(const Packet &packet);

public:
	virtual bool hasListener(Connection::Listener::Pointer listener);
	virtual bool addListener(Connection::Listener::Pointer listener);
	virtual bool removeListener(Connection::Listener::Pointer listener);
	virtual void clearListeners();

protected:
	TCP::Client::Pointer _tcp_client;
	UDP::Client::Pointer _udp_client;
}; // class BasicClient

typedef std::list<BasicClient::Pointer> BasicClient_l;

} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORK_BASICCLIENT_H
