#include "Connection.h"

using namespace RSSD;
using namespace RSSD::Network;

///
/// @class Connection
///

Connection::Connection(const int32_t protocol) :
	Loggable(Connection::getBuiltinLogGroup()),
	_is_open(false),
	_is_threaded(true),
	_listen_port(0),
	_remote_port(0),
	_protocol(protocol),
	_id(0),
	_type(Connection::UNSPECIFIED),
	_listen_address(RSSD::Network::ANY_ADDRESS),
	_remote_address(RSSD::Network::LOOPBACK_ADDRESS),
	_service(new bnet::io_service())
{
	this->init();
}

Connection::Connection(
	io_service_ptr service,
	const int32_t protocol) :
	Loggable(Connection::getBuiltinLogGroup()),
	_is_open(false),
	_is_threaded(false),
	_listen_port(0),
	_remote_port(0),
	_protocol(protocol),
	_id(0),
	_type(Connection::UNSPECIFIED),
	_listen_address(RSSD::Network::ANY_ADDRESS),
	_remote_address(RSSD::Network::LOOPBACK_ADDRESS),
	_service(service)
{
	this->init();
}

Connection::~Connection()
{
	this->close();
}

void Connection::updateLogGroup()
{
	// Setup base log group name
	std::stringstream ss;
	ss << Connection::getBuiltinLogGroup() << "/" << this->_id;

	// Append protocol type
	switch (this->_protocol)
	{
		case SOCK_STREAM: { ss << ":TCP"; break; }
		case SOCK_DGRAM: { ss << ":UDP"; break; }
		case SOCK_RAW: { ss << ":ICMP"; break; }
		default: { ss << ":UNKNOWN"; break; }
	}

	// Append connection-side type
	switch (this->_type)
	{
		case Connection::CLIENT: { ss << "|CLIENT"; break; }
		case Connection::SERVER: { ss << "|SERVER"; break; }
		case Connection::UNSPECIFIED:
		default: break;
	}

	// Invoke base class method
	Loggable::setLogGroup(ss.str());
}

void Connection::init()
{
	// Initialize connection ID
	this->setId(0);
}

bool Connection::open()
{
	/// @validate Connection is closed.
	if (this->isOpen())
	{
        NLOG ("Connection open.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return false;
	}

	// Begin connection-specific main loop
	if (this->_is_threaded)
		this->run();

	// Update connection state flag
	this->_is_open = true;
	return true;
}

void Connection::run()
{
	// Begin threaded execution loop
	this->_thread.reset(
		new boost::thread(
			boost::bind(
				&Connection::onRun,
				this->getPointer())));
}

void Connection::onRun()
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

	assert (this->_is_threaded);
	while (this->_is_open)
	{
		try { this->_service->run(); }
        catch (const std::exception &e) { NLOG (e.what(), this->getLogGroup().c_str(), Log::Log::Level::ERROR); }
		this->_service->reset();
	}
    NLOG ("Exited main loop.", this->getLogGroup().c_str(), Log::Level::DEBUG);
	this->_stop_condition.notify_one();
}

bool Connection::close()
{
	/// @validate Connection is open.
	if (!this->isOpen())
	{
        NLOG ("Connection closed.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return false;
	}

	// Clear packet queues
	while (!this->_sendq.empty()) this->_sendq.pop();
	while (!this->_recvq.empty()) this->_recvq.pop();

	if (this->_is_threaded)
	{
		this->_service->stop();
        NLOG ("Stopping connection IO service...", this->getLogGroup().c_str(), Log::Level::DEBUG);
		boost::mutex::scoped_lock lock(this->_mutex);
        NLOG ("Waiting on IO service to stop...", this->getLogGroup().c_str(), Log::Level::DEBUG);
		this->_stop_condition.wait(lock);
        NLOG ("Stopped IO service!", this->getLogGroup().c_str(), Log::Level::DEBUG);
	}
	return true;
}

bool Connection::hasListener(Connection::Listener::Pointer listener)
{
	return this->_listener_manager.has(listener);
}

bool Connection::addListener(Connection::Listener::Pointer listener)
{
    NLOG ("Adding listener to connection.", this->getLogGroup().c_str(), Log::Level::INSANE);
	return this->_listener_manager.add(listener);
}

bool Connection::removeListener(Connection::Listener::Pointer listener)
{
    NLOG ("Removing listener from connection.", this->getLogGroup().c_str(), Log::Level::INSANE);
	return this->_listener_manager.remove(listener);
}

void Connection::clearListeners()
{
	this->_listener_manager.clear();
}

///
/// @class Connection::Listener
///

Connection::Listener::Listener(const bool queue_packets) :
	Loggable(Connection::Listener::getBuiltinLogGroup()),
	_is_queue_packets(queue_packets)
{

}

Connection::Listener::~Listener()
{

}

void Connection::Listener::onConnectionOpened(Connection::Pointer connection)
{
	try
	{
        NLOG ("Opened connection to: " << connection->getId(), this->getLogGroup().c_str(), Log::Level::DEBUG);
	}
	catch (const boost::system::error_code ec)
	{
        NLOG (ec.message(), this->getLogGroup().c_str(), Log::Log::Level::ERROR);
	}
}

void Connection::Listener::onConnectionClosed(Connection::Pointer connection)
{
	try
	{
        NLOG ("Closed connection to: " << connection->getId(), this->getLogGroup().c_str(), Log::Level::DEBUG);
	}
	catch (const boost::system::error_code ec)
	{
        NLOG (ec.message(), this->getLogGroup().c_str(), Log::Log::Level::ERROR);
	}
}

void Connection::Listener::onPacketReceived(Connection::Pointer connection, const Packet &packet)
{
	try
	{
        NLOG ("Received packet from: " << connection->getId(), this->getLogGroup().c_str(), Log::Level::DEBUG);
        NLOG ("Packet contents: " << packet, this->getLogGroup().c_str(), Log::Level::INSANE);
	}
	catch (const boost::system::error_code ec)
	{
        NLOG (ec.message(), this->getLogGroup().c_str(), Log::Log::Level::ERROR);
	}

	// Store packet in thread-safe message queue
	if (this->_is_queue_packets)
	{
		this->_packets.push(packet);
        NLOG ("Storing packet in thread-safe message queue (size=" << this->_packets.size() << ").", this->getLogGroup().c_str(), Log::Level::INSANE);
        NLOG ("Stored packet with contents: " << packet, this->getLogGroup().c_str(), Log::Level::INSANE);
	}
}

void Connection::Listener::onPacketSent(Connection::Pointer connection, const Packet &packet)
{
	try
	{
        NLOG ("Sent packet to: " << connection->getId(), this->getLogGroup().c_str(), Log::Level::DEBUG);
        NLOG ("Packet contents: " << packet, this->getLogGroup().c_str(), Log::Level::INSANE);
	}
	catch (const boost::system::error_code ec)
	{
        NLOG (ec.message(), this->getLogGroup().c_str(), Log::Log::Level::ERROR);
	}
}
