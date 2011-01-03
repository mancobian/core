#include "Server.h"

using namespace RSSD;
using namespace RSSD::Network;

Server::Server(const string_t &log_group) :
	Connection::Listener(),
	ConnectionManager(),
	ConnectionListenerManager(),
	_is_running(false),
	_is_threaded(false),
	_service(new bnet::io_service())
{
	this->init(log_group);
}

Server::Server(
	io_service_ptr service,
	const string_t &log_group) :
	Connection::Listener(),
	ConnectionManager(),
	ConnectionListenerManager(),
	_is_running(false),
	_is_threaded(false),
	_service(service)
{
	this->init(log_group);
}

Server::~Server()
{
	this->stop();
}

void Server::init(const string_t &log_group)
{
	this->setLogGroup(log_group);
}

bool Server::start()
{
	return this->start(false);
}

bool Server::start(const bool threaded)
{
	// @validate Server is not running
	if (this->_is_running)
		return false;
	this->_is_running = true;
	this->_is_threaded = threaded;
	if (threaded)
	{
		// Begin threaded listener loop
		this->_thread.reset(new boost::thread(
			boost::bind(
				&Server::onStart,
				this->getPointer())));
	}
	return this->_is_running;
}

void Server::onStart()
{
	///
	/// @note This will keep the server running
	/// in the face of any socket level exceptions,
	/// stopping the server loop only when
	/// explicitly requested.
	///
	/// For further reference:
	///	http://www.boost.org/doc/libs/1_37_0/doc/html/boost_asio/reference/io_service.html
	///

	while (this->_is_running)
	{
		try { this->_service->run(); this->_service->reset(); }
        catch (const std::exception &e) { NLOG (e.what(), this->getLogGroup().c_str(), Log::Log::Level::ERROR); }
	}
	this->_stop_condition.notify_one();
}

bool Server::stop()
{
	// @validate Server is running
	if (!this->isRunning())
		return false;
	this->_is_running = false;

	// Close all managed connections
	Connection_l::iterator
		iter = ConnectionManager::_items.begin(),
		end = ConnectionManager::_items.end();
	while (iter != end)
	{
		ConnectionManager::Item connection = *iter++;
		connection->close();
        NLOG ("Closed server-side connection <" << connection->getId() << ">", this->getLogGroup().c_str(), Log::Level::DEBUG);
	}

	///
	/// @note This resets the service object, not the shared_ptr!
	/// Resetting the IO service object is required to allow a
	/// user to start the server again after stopping it.
	///

	if (this->_is_threaded)
	{
		this->_service->stop();
		boost::mutex::scoped_lock lock(this->_mutex);
		this->_stop_condition.wait(lock);
	}
	return true;
}

bool Server::send(
	const Packet &packet,
	Connection::Pointer connection)
{
	if (!ConnectionManager::has(connection))
		return false;
	this->_service->post(
		boost::bind(
			&Server::onSend,
			this->getPointer(),
			boost::cref(packet),
			connection));
	return true;
}

bool Server::broadcast(const Packet &packet)
{
	this->_service->post(
		boost::bind(
			&Server::onSend,
			this->getPointer(),
			boost::cref(packet),
			Connection::Pointer()));
	return true;
}

void Server::onSend(
	const Packet &packet,
	Connection::Pointer connection)
{
	// Send packet to single connection
	if (connection.get() != NULL)
	{
		connection->send(packet);
		return;
	}
	// Broadcast packet to all managed connection
	else
	{
		Connection_l::iterator
			iter = ConnectionManager::_items.begin(),
			end = ConnectionManager::_items.end();
		for (; iter != end; ++iter)
		{
			ConnectionManager::Item connection = *iter;
			connection->send(packet);
            NLOG ("Sent packet to connection <" << connection->getId() << ">: " << packet, this->getLogGroup().c_str(), Log::Level::INSANE);
		}
	}
}

bool Server::addListener(ConnectionListener item)
{
	return ConnectionListenerManager::add(item);
}

bool Server::removeListener(ConnectionListener item)
{
	return ConnectionListenerManager::remove(item);
}

void Server::clearListeners()
{
	ConnectionListenerManager::clear();
}

void Server::onConnectionOpened(Connection::Pointer connection)
{
	ConnectionListener_l::iterator
		iter = ConnectionListenerManager::_items.begin(),
		end = ConnectionListenerManager::_items.end();
	for (; iter != end; ++iter)
	{
		Connection::Listener::Pointer &listener = *iter;
		listener->onConnectionOpened(connection);
	}
}

void Server::onConnectionClosed(Connection::Pointer connection)
{
	ConnectionListener_l::iterator
		iter = ConnectionListenerManager::_items.begin(),
		end = ConnectionListenerManager::_items.end();
	for (; iter != end; ++iter)
	{
		Connection::Listener::Pointer &listener = *iter;
		listener->onConnectionClosed(connection);
	}
}

void Server::onPacketReceived(Connection::Pointer connection, const Packet &packet)
{
	ConnectionListener_l::iterator
		iter = ConnectionListenerManager::_items.begin(),
		end = ConnectionListenerManager::_items.end();
	for (; iter != end; ++iter)
	{
		Connection::Listener::Pointer &listener = *iter;
		listener->onPacketReceived(connection, packet);
	}
}

void Server::onPacketSent(Connection::Pointer connection, const Packet &packet)
{
	ConnectionListener_l::iterator
		iter = ConnectionListenerManager::_items.begin(),
		end = ConnectionListenerManager::_items.end();
	for (; iter != end; ++iter)
	{
		Connection::Listener::Pointer &listener = *iter;
		listener->onPacketSent(connection, packet);
	}
}
