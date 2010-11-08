#include "BasicClientManager.h"
#include <network/test/BasicMessage.h>

using namespace RSSD;
using namespace RSSD::Network;

BasicClientManager::BasicClientManager(
	const uint32_t tcp_port,
	const uint32_t udp_client_port,
	const uint32_t udp_server_port,
	const std::string &udp_broadcast_address,
	const uint32_t udp_broadcast_port,
	const uint32_t num_clients) :
	RSSD::Pattern::Manager<BasicClient::Pointer>(),
	_is_running(false),
	_tcp_port(tcp_port),
	_udp_client_port(udp_client_port),
	_udp_server_port(udp_server_port),
	_udp_broadcast_port(udp_broadcast_port),
	_num_clients(num_clients),
	_udp_broadcast_address(udp_broadcast_address),
	_service(new bnet::io_service())
{

}

BasicClientManager::~BasicClientManager()
{
	this->onStop();
}

void BasicClientManager::start()
{
	boost::mutex::scoped_lock lock(this->_mutex);
	if (this->_is_running)
		return;

	this->_is_running = true;
	this->createClients();

	// Begin client thread
	boost::thread t(
		boost::bind(
			&BasicClientManager::onStart,
			this->getPointer()));
}

void BasicClientManager::onStart()
{
	while (this->_is_running)
	{
		try { this->_service->run(); }
		catch (const std::exception &e) { NLOG (e.what(), BasicClientManager::getBuiltinLogGroup(), Log::Level::ERROR); }
		this->_service->reset();
		// boost::this_thread::sleep(boost::posix_time::milliseconds(1));
	}
}

void BasicClientManager::stop()
{
	boost::mutex::scoped_lock lock(this->_mutex);
	if (!this->_is_running)
		return;
	this->_service->post(
		boost::bind(
			&BasicClientManager::onStop,
			this->getPointer()));
}

void BasicClientManager::onStop()
{
	this->destroyClients();
	this->_service->stop();
	this->_is_running = false;
	this->_service->reset();
}

void BasicClientManager::send()
{
	this->_service->post(
		boost::bind(
			&BasicClientManager::onSend,
			this->getPointer()));
}

void BasicClientManager::onSend()
{
	BasicClient_l::iterator
		iter = this->_items.begin(),
		end = this->_items.end();
	for (; iter != end; ++iter)
	{
		BasicClient::Pointer client = *iter;
		BasicMessage message(7, "Happy Time!");
		client->send(message, randbool());
		// client->send(message, true);
	}
}

void BasicClientManager::createClients()
{
	for (uint32_t i=0; i<this->_num_clients; ++i)
	{
		BasicClient::Pointer client(new BasicClient(this->_service));
		client->setId(i+1);
		client->setUdpListenPort(this->_udp_client_port);
		client->setTcpRemoteAddress(RSSD::Network::LOOPBACK_ADDRESS.to_string());
		client->setTcpRemotePort(this->_tcp_port);
		client->setUdpRemoteAddress(RSSD::Network::LOOPBACK_ADDRESS.to_string());
		client->setUdpRemotePort(this->_udp_server_port);
		client->setUdpBroadcastAddress(RSSD::Network::LOOPBACK_ADDRESS.to_string());
		client->setUdpBroadcastPort(this->_udp_broadcast_port);
		client->open();
		client->addListener(this->shared_from_this());
		this->add(client);
	}

	NLOG ("Created and managing " << this->size() << " connections.", BasicClientManager::getBuiltinLogGroup(), Log::Level::DEBUG);
}

void BasicClientManager::destroyClients()
{
	boost::mutex::scoped_lock lock(this->_mutex);
	BasicClient_l::iterator
		iter = this->_items.begin(),
		end = this->_items.end();
	for (; iter != end; ++iter)
	{
		BasicClient::Pointer client = *iter;
		client->close();
		NLOG ("Closing connection: " << client->getId(), BasicClientManager::getBuiltinLogGroup());
	}

	if (!this->_items.empty())
		this->_stop_condition.wait(lock);
}

void BasicClientManager::onConnectionOpened(Connection::Pointer connection)
{
	// NLOG (this->size() << " connections opened.", BasicClientManager::getBuiltinLogGroup());
}

void BasicClientManager::onConnectionClosed(Connection::Pointer connection)
{
	NLOG ("Waiting on " << this->size() << " connections to close...", BasicClientManager::getBuiltinLogGroup());
	BasicClient::Pointer client = boost::dynamic_pointer_cast<BasicClient>(connection);
	this->remove(client);

	if (this->size() == 0)
		this->_stop_condition.notify_one();
}

void BasicClientManager::onPacketReceived(Connection::Pointer connection, const Packet &packet)
{

}

void BasicClientManager::onPacketSent(Connection::Pointer connection, const Packet &packet)
{

}
