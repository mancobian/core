#ifndef NOUS_NETWORKING_BASICCLIENTMANAGER_HPP
#define NOUS_NETWORKING_BASICCLIENTMANAGER_HPP

#include <pattern/Manager.h>
#include <network/Common.h>
#include <network/BasicClient.h>

namespace RSSD {
namespace Network {

class BasicClientManager :
	public boost::enable_shared_from_this<BasicClientManager>,
	public Pattern::Manager<BasicClient::Pointer>,
	public Connection::Listener
{
public:
	typedef RSSD::Pattern::Manager<RSSD::Network::BasicClient::Pointer> BaseManager;
	typedef BaseManager::Item_l BasicClient_l;
	typedef boost::shared_ptr<BasicClientManager> Pointer;

public:
	BasicClientManager(
		const uint32_t tcp_port,
		const uint32_t udp_client_port,
		const uint32_t udp_server_port,
		const std::string &udp_broadcast_address,
		const uint32_t udp_broadcast_port,
		const uint32_t num_clients = 0);
	virtual ~BasicClientManager();

public:
	inline BasicClientManager::Pointer getPointer() { return this->shared_from_this(); }
	inline uint32_t getNumClients() const { return this->_num_clients; }
	inline void setNumClients(const uint32_t value) { this->_num_clients = value; }

public:
	void start();
	void stop();
	void send();

protected:
	static const char* getBuiltinLogGroup() { return "NET/BCLIENTMGR"; }

protected:
	void onStart();
	void onStop();
	void onSend();
	void createClients();
	void destroyClients();

protected:
	virtual void onConnectionOpened(Connection::Pointer connection);
	virtual void onConnectionClosed(Connection::Pointer connection);
	virtual void onPacketReceived(Connection::Pointer connection, const Packet &packet);
	virtual void onPacketSent(Connection::Pointer connection, const Packet &packet);

protected:
	volatile bool _is_running;
	uint32_t _tcp_port,
		_udp_client_port,
		_udp_server_port,
		_udp_broadcast_port,
		_num_clients;
	std::string _udp_broadcast_address;
	boost::mutex _mutex;
	boost::condition_variable _stop_condition;
	io_service_ptr _service;
}; // class BasicClientManager

} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORKING_BASICCLIENTMANAGER_HPP
