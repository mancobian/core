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

#ifndef NOUS_NETWORK_SERVER_H
#define NOUS_NETWORK_SERVER_H

#include <network/Common.h>
#include <network/Connection.h>

namespace RSSD {
namespace Network {

class Server :
	public boost::enable_shared_from_this<Server>,
	public Connection::Listener,
	public ConnectionManager,
	public ConnectionListenerManager
{
public:
	typedef boost::shared_ptr<Server> Pointer;

public:
	Server(const std::string &log_group = Server::getBuiltinLogGroup());
	Server(
		io_service_ptr service,
		const std::string &log_group = Server::getBuiltinLogGroup());
	virtual ~Server();

public:
	inline Server::Pointer getPointer() { return this->shared_from_this(); }
	virtual inline bool isRunning() { boost::mutex::scoped_lock lock(this->_mutex); return this->_is_running; }
	virtual inline io_service_ptr getIoService() const { return this->_service; }

public:
	virtual bool start() = 0;
	virtual bool stop() = 0;
	virtual bool start(const bool threaded);
	virtual bool send(
		const Packet &packet,
		Connection::Pointer connection);
	virtual bool broadcast(const Packet &packet);
	virtual bool addListener(ConnectionListener listener);
	virtual bool removeListener(ConnectionListener listener);
	virtual void clearListeners();

protected:
	static const char* getBuiltinLogGroup() { return "NET/SERVER"; }

protected:
	virtual void init(const std::string &log_group);
	virtual void onStart();
	virtual void onSend(
		const Packet &packet,
		Connection::Pointer connection);

protected:
	virtual void onConnectionOpened(Connection::Pointer connection);
	virtual void onConnectionClosed(Connection::Pointer connection);
	virtual void onPacketReceived(Connection::Pointer connection, const Packet &packet);
	virtual void onPacketSent(Connection::Pointer connection, const Packet &packet);

protected:
	volatile bool _is_running,
		_is_threaded;
	boost::mutex _mutex;
	boost::condition_variable _stop_condition;
	thread_ptr _thread;
	io_service_ptr _service;
}; // class Server

} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORK_SERVER_H
