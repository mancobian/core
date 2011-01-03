///
/// @file BasicServer.h
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

#ifndef NOUS_NETWORK_BASICSERVER_H
#define NOUS_NETWORK_BASICSERVER_H

#include <network/Common.h>
#include <network/Server.h>
#include <network/tcp/Server.h>
#include <network/udp/Server.h>

namespace RSSD {
namespace Network {

class BasicServer : public Server
{
public:
	typedef boost::shared_ptr<BasicServer> Pointer;

public:
	BasicServer(
		const unsigned short tcp_listen_port,
		const unsigned short udp_listen_port,
		const unsigned short udp_broadcast_port = 0,
		const string_t &log_group = BasicServer::getBuiltinLogGroup());
	virtual ~BasicServer();

public:
	virtual bool start();
	virtual bool stop();
	virtual bool addListener(ConnectionListener item);
	virtual bool removeListener(ConnectionListener item);
	virtual void clearListeners();

protected:
	static const char* getBuiltinLogGroup() { return "NET/BSERVER"; }

protected:
	TCP::Server::Pointer _tcp_server;
	UDP::Server::Pointer _udp_server;
}; // namespace BasicServer

} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORK_BASICSERVER_H

