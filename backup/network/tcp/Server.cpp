#include "Server.h"

using namespace RSSD;
using namespace RSSD::Network;

TCP::Server::Server(
	io_service_ptr service,
	const uint16_t listen_port,
	const string_t &log_group) :
	RSSD::Network::Server(service, log_group),
	_client_id(1),
	_listen_endpoint(bnet::ip::address_v4::any(), listen_port),
	_acceptor(*service)
{
}

TCP::Server::~Server()
{
}

bool TCP::Server::start()
{
	// Invoke base class method
	if (!RSSD::Network::Server::start())
	{
		assert (false);
		return false;
	}

	/// @validate Non-zero value required for socket port.
	if (!this->_listen_endpoint.port())
	{
		NLOG ("Invalid port.", this->getLogGroup().c_str(), Log::Level::INSANE);
		return false;
	}

	// Begin listener loop
	this->onStart();
	return true;
}

void TCP::Server::onStart()
{
	// Begin the async accept loop
	NLOG ("Beginning listen(...) for server endpoint: " << this->_listen_endpoint, this->getLogGroup().c_str());
	this->_acceptor.open(this->_listen_endpoint.protocol());
	this->_acceptor.set_option(bnet::ip::tcp::acceptor::reuse_address(true));
	this->_acceptor.bind(this->_listen_endpoint);
	this->_acceptor.listen();
	this->listen();
}

///
/// @todo Implement connection threading via threadpool
///

void TCP::Server::listen(const boost::system::error_code &error)
{
	// Validate: If an error occurred or we have
	// explicitly stopped the server, then short-circuit
	// the listener loop here
	if (error || !this->_is_running)
	{
		this->stop();
		return;
	}

	// Spawn thread to handle the new connection
	/// @note This will not execute on the first pass through this method.
	if (this->_pending_connection)
	{
		// Register this server as a connection listener
		/// @hack Servers are a special case where we must invoke the
		/// onConnectionOpened(...) event handler manually, since sockets
		/// are opened remotely by the accept(...) process and not by the
		/// user.
		if (this->_pending_connection->receive())
			this->onConnectionOpened(this->_pending_connection);
	}

	// Accept incoming connection
	/// @note This function is called again to complete the
	/// acceptance process, which subsequently sets the server
	/// up to listen for the next connection attempt -- this
	/// is a callback loop.
	this->_pending_connection.reset(
		new TCP::Connection(this->_service));
	this->_pending_connection->setId(this->_client_id++);
	this->_pending_connection->setType(Connection::SERVER);

	// "Wait" on new connection...
	// NLOG ("Waiting to accept(...) connection named <" << this->_pending_connection->getId() << ">", this->getLogGroup().c_str());
	this->_acceptor.async_accept(
		this->_pending_connection->getSocket(),
		boost::bind(
			&Server::listen,
			this->getPointer(),
			bnet::placeholders::error));
}

bool TCP::Server::stop()
{
	if (!this->isRunning())
		return false;

	this->onStop();
	return true;
}

void TCP::Server::onStop()
{
	// Invoke base class method
	boost::system::error_code error;
	this->_acceptor.cancel(error);
	// assert (error == NULL);
	this->_acceptor.close(error);
	// assert (error == NULL);
	this->_pending_connection.reset();
	RSSD::Network::Server::stop();
}

void TCP::Server::onConnectionOpened(Connection::Pointer connection)
{
	connection->addListener(this->shared_from_this());
	assert (ConnectionManager::add(connection));
	NLOG ("Added connection: " << connection->getId() << " (" << ConnectionManager::_items.size() << " connections)", this->getLogGroup().c_str(), Log::Level::DEBUG);
	RSSD::Network::Server::onConnectionOpened(connection);
}

void TCP::Server::onConnectionClosed(Connection::Pointer connection)
{
	assert (ConnectionManager::remove(connection));
	NLOG ("Added connection: " << connection->getId() << "(" << ConnectionManager::_items.size() << " connections)", this->getLogGroup().c_str(), Log::Level::DEBUG);
	RSSD::Network::Server::onConnectionClosed(connection);
}

void TCP::Server::onPacketReceived(Connection::Pointer connection, const Packet &packet)
{
	NLOG ("Received packet from <" << connection->getId() << ">: " << packet << ".", this->getLogGroup().c_str(), Log::Level::INSANE);
	RSSD::Network::Server::onPacketReceived(connection, packet);
}

void TCP::Server::onPacketSent(Connection::Pointer connection, const Packet &packet)
{
	NLOG ("Received packet from <" << connection->getId() << ">: " << packet << ".", this->getLogGroup().c_str(), Log::Level::INSANE);
	RSSD::Network::Server::onPacketSent(connection, packet);
}
