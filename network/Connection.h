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

#ifndef NOUS_NETWORK_CONNECTION_H
#define NOUS_NETWORK_CONNECTION_H

#include <pattern/Manager.h>
#include <network/Common.h>
#include <network/Packet.h>

namespace RSSD {
namespace Network {

namespace TCP { class Server; }
namespace UDP { class Server; }

class Connection :
	public boost::enable_shared_from_this<Connection>,
	public Loggable
{
public:
	typedef boost::shared_ptr<Connection> Pointer;
	typedef std::queue<Packet> Send_q;
	typedef std::queue<byte_v> Receive_q;
	typedef boost::asio::ip::address IpAddress;

public:
	enum Type
	{
		UNSPECIFIED = 0,
		CLIENT = 1<<0,
		SERVER = 1<<1
	}; // enum Type

public:
	class Listener : public Loggable
	{
	public:
		typedef boost::shared_ptr<Listener> Pointer;

	public:
		Listener(const bool queue_packets = false);
		virtual ~Listener();

	public:
		PacketQueue& getPacketQueue() { return this->_packets; }

	public:
		virtual void onConnectionOpened(Connection::Pointer connection);
		virtual void onConnectionClosed(Connection::Pointer connection);
		virtual void onPacketReceived(Connection::Pointer connection, const Packet &packet);
		virtual void onPacketSent(Connection::Pointer connection, const Packet &packet);

	protected:
		static const char* getBuiltinLogGroup() { return "NET/CONNECTION/LISTENER"; }

	protected:
		bool _is_queue_packets;
		PacketQueue _packets;
	}; // class Listener

public:
	Connection(const int32_t protocol = SOCK_RAW);
	Connection(
		io_service_ptr service,
		const int32_t protocol = SOCK_RAW);
	virtual ~Connection();

public:
	inline Connection::Pointer getPointer() { return this->shared_from_this(); }
	virtual inline bool isOpen() const { return this->_is_open; }
	virtual inline uint32_t getId() const { return this->_id; }
	virtual inline void setId(const uint32_t value) { this->_id = value; this->updateLogGroup(); }
	virtual inline Type getType() const { return this->_type; }
	virtual inline void setType(const Type value) { this->_type = value; this->updateLogGroup(); }

public:
	virtual bool open() = 0;
	virtual bool close() = 0;
	virtual bool receive() = 0;
	virtual bool send(const Packet &packet) = 0;

public:
	virtual bool hasListener(Listener::Pointer listener);
	virtual bool addListener(Listener::Pointer listener);
	virtual bool removeListener(Listener::Pointer listener);
	virtual void clearListeners();

protected:
	static const char* getBuiltinLogGroup() { return "NET/CONNECTION"; }

protected:
	virtual void updateLogGroup();
	virtual void init();
	virtual void run();
	virtual void onRun();

protected:
	volatile bool _is_open;
	bool _is_threaded;
	uint16_t
		_listen_port,
		_remote_port;
	int32_t _protocol;
	uint32_t _id;
	Type _type;
	boost::mutex _mutex;
	boost::condition_variable _stop_condition;
	thread_ptr _thread;
	bnet::streambuf _buffer;
	IpAddress
		_listen_address,
		_remote_address;
	Send_q _sendq;
	Receive_q _recvq;
	io_service_ptr _service;
	Pattern::Manager<Listener::Pointer> _listener_manager;
}; // class Connection

///
/// Typedefs
///

typedef RSSD::Pattern::Manager<Connection::Pointer> ConnectionManager;
typedef ConnectionManager::Item_l Connection_l;
typedef RSSD::Pattern::Manager<Connection::Listener::Pointer> ConnectionListenerManager;
typedef RSSD::Pattern::Manager<Connection::Listener::Pointer>::Item ConnectionListener;
typedef ConnectionListenerManager::Item_l ConnectionListener_l;

} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORK_CONNECTION_H
