#include "Server.h"

using namespace RSSD;
using namespace RSSD::Network;

UDP::Server::Server(
	io_service_ptr service,
	const uint16_t listen_port,
	const uint16_t remote_port,
	const std::string &log_group) :
	RSSD::Network::Server(service, log_group),
	_connection(UDP::Connection::Pointer(new UDP::Connection(service)))
{
	// Configure UDP listener connection
	this->_connection->setListenAddress(RSSD::Network::ANY_ADDRESS.to_string());
	this->_connection->setListenPort(listen_port);
	this->_connection->setRemoteAddress(RSSD::Network::BROADCAST_ADDRESS.to_string());
	this->_connection->setRemotePort(remote_port);
	this->_connection->setBroadcastAddress(RSSD::Network::LOOPBACK_ADDRESS.to_string());
	this->_connection->setBroadcastPort(remote_port);
	this->_connection->setType(Connection::SERVER);
}

UDP::Server::~Server()
{
}

bool UDP::Server::start()
{
	NLOG ("Starting UDP server...", this->getLogGroup().c_str(), Log::Level::DEBUG);
	if (this->isRunning())
	{
		NLOG ("Server is running.", this->getLogGroup().c_str(), Log::Level::ERROR);
		return false;
	}
	this->onStart();
	return true;
}

void UDP::Server::onStart()
{
	// Invoke base class method
	NLOG ("Starting base server from UDP server...", this->getLogGroup().c_str(), Log::Level::INSANE);
	assert (Network::Server::start());
	this->_connection->addListener(this->shared_from_this());
	NLOG ("Opening UDP server listener/sender connection...", this->getLogGroup().c_str(), Log::Level::INSANE);
	this->_connection->open();
}

bool UDP::Server::stop()
{
	if (!this->isRunning())
		return false;

	this->onStop();
	return true;
}

void UDP::Server::onStop()
{
	this->_connection->close();
	this->_connection->removeListener(this->shared_from_this());

	// Invoke base class method
	RSSD::Network::Server::stop();
}

bool UDP::Server::broadcast(const Packet &packet)
{
	this->_connection->broadcast(packet);
	return true;
}

void UDP::Server::onConnectionOpened(RSSD::Network::Connection::Pointer connection)
{
	// assert (ConnectionManager::add(connection));
	NLOG ("Opened connection <" << connection->getId() << ">.", this->getLogGroup().c_str(), Log::Level::DEBUG);
	// NLOG ("Added server as connection <" << connection->getId() << "> listener.", this->getLogGroup().c_str());
	// RSSD::Network::Server::onConnectionOpened(connection);
}

void UDP::Server::onConnectionClosed(RSSD::Network::Connection::Pointer connection)
{
	// assert (ConnectionManager::remove(connection));
	NLOG ("Closed connection <" << connection->getId() << ">.", this->getLogGroup().c_str(), Log::Level::DEBUG);
	// NLOG ("Removed server as connection <" << connection->getId() << "> listener.", this->getLogGroup().c_str());
	// RSSD::Network::Server::onConnectionClosed(connection);
}

void UDP::Server::onPacketReceived(Connection::Pointer connection, const Packet &packet)
{
	NLOG ("Received a packet: " << packet, this->getLogGroup().c_str(), Log::Level::INSANE);
	RSSD::Network::Server::onPacketReceived(connection, packet);
}

void UDP::Server::onPacketSent(Connection::Pointer connection, const Packet &packet)
{
	NLOG ("Sent a packet: " << packet, this->getLogGroup().c_str(), Log::Level::INSANE);
	RSSD::Network::Server::onPacketSent(connection, packet);
}
