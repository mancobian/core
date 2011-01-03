///
/// @file InternetConnection.h
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

#ifndef NOUS_NETWORK_INTERNETCONNECTION_H
#define NOUS_NETWORK_INTERNETCONNECTION_H

#include <network/Connection.h>

namespace RSSD {
namespace Network {

template <typename PROTOCOL>
class InternetConnection : public Connection
{
public:
	typedef boost::shared_ptr<InternetConnection<PROTOCOL> > Pointer;
	typedef typename PROTOCOL::socket Socket;
	typedef typename boost::asio::ip::basic_endpoint<PROTOCOL> Endpoint;
	typedef typename boost::asio::ip::basic_resolver<PROTOCOL> Resolver;
	typedef typename boost::shared_ptr<Endpoint> EndpointPointer;

public:
	InternetConnection();
	InternetConnection(io_service_ptr service);
	virtual ~InternetConnection();

public:
	inline typename InternetConnection<PROTOCOL>::Pointer getPointer() { return boost::dynamic_pointer_cast<InternetConnection<PROTOCOL> >(this->shared_from_this()); }
	virtual inline bool isOpen() { boost::mutex::scoped_lock lock(this->_mutex); return this->_socket.is_open(); }
	virtual inline bool isOpen() const { return this->_socket.is_open(); }
	virtual inline Socket& getSocket() { return this->_socket; }
	virtual string_t getEndpointsAsString() const;

	virtual inline string_t getListenAddress() const { return this->_listen_address.to_string(); }
	virtual inline void setListenAddress(const string_t &value) { this->_listen_address = bnet::ip::address::from_string(value); }
	virtual inline unsigned short getListenPort() const { return this->_listen_port; }
	virtual inline void setListenPort(const unsigned short value) { this->_listen_port = value; }

	virtual inline string_t getRemoteAddress() const { return this->_remote_address.to_string(); }
	virtual inline void setRemoteAddress(const string_t &value) { this->_remote_address = bnet::ip::address::from_string(value); }
	virtual inline unsigned short getRemotePort() const { return this->_remote_port; }
	virtual inline void setRemotePort(const unsigned short value) { this->_remote_port = value; }

public:
	virtual bool open();
	virtual bool open(const bool is_datagram);
	virtual bool close();

protected:
	virtual void init();
	virtual bool setSocketOptions();
	virtual bool onOpen(const bool is_datagram);
	virtual void onConnect(
		typename Resolver::iterator endpoint_iterator,
		const boost::system::error_code &error);
	virtual bool updateEndpoints();
	virtual bool updateListenEndpoint();
	virtual bool updateRemoteEndpoint();

protected:
	EndpointPointer
		_listen_endpoint,
		_remote_endpoint;
	Socket _socket;

private:
	friend class RSSD::Network::TCP::Server;
	friend class RSSD::Network::UDP::Server;
}; // class InternetConnection

///
/// Include Files
///

#include "InternetConnection-inl.h"

} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORK_INTERNETCONNECTION_H
