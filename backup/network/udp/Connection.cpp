#include "Connection.h"

using namespace RSSD;
using namespace RSSD::Network;

UDP::Connection::Connection() :
	BaseConnection(),
	_broadcast_port(0),
	_broadcast_address(Network::BROADCAST_ADDRESS)
{
}

UDP::Connection::Connection(io_service_ptr service) :
	BaseConnection(service),
	_broadcast_port(0),
	_broadcast_address(Network::BROADCAST_ADDRESS)
{
}

UDP::Connection::~Connection()
{
	this->close();
}

bool UDP::Connection::setSocketOptions()
{
	NLOG ("Setting socket options...", this->getLogGroup().c_str(), Log::Level::INSANE);
	this->_socket.set_option(Socket::broadcast(true));
	this->_socket.set_option(Socket::receive_buffer_size(Packet::MAX_UDP_SIZE));
	this->_socket.set_option(Socket::send_buffer_size(Packet::MAX_UDP_SIZE));
	return BaseConnection::setSocketOptions();
}

string_t UDP::Connection::getEndpointsAsString() const
{
	std::stringstream output;
	string_t base_endpoints = BaseConnection::getEndpointsAsString();
	output << base_endpoints;
	if (this->_broadcast_endpoint)
		output << "(Broadcast) " << this->_broadcast_endpoint << " ";
	return output.str();
}

bool UDP::Connection::updateEndpoints()
{
	NLOG ("Updating endpoints...", this->getLogGroup().c_str(), Log::Level::INSANE);
	bool result = BaseConnection::updateEndpoints();
	result |= this->updateBroadcastEndpoint();
	return result;
}

bool UDP::Connection::updateBroadcastEndpoint()
{
	/// @validate Non-zero value required for socket port.
	if (!this->_broadcast_port)
	{
		NLOG ("Invalid port.", this->getLogGroup().c_str(), Log::Level::INSANE);
		return false;
	}

	/// @validate Non-empty value required for socket address.
	if (this->_broadcast_address.to_string().empty())
	{
		NLOG ("Invalid IP address.", this->getLogGroup().c_str(), Log::Level::INSANE);
		return false;
	}

	this->_broadcast_endpoint.reset(
		new BaseConnection::Endpoint(
			this->_broadcast_address,
			this->_broadcast_port));
	return true;
}

bool UDP::Connection::open()
{
	NLOG ("Opening connection...", this->getLogGroup().c_str(), Log::Level::INSANE);
	return BaseConnection::open(true);
}

bool UDP::Connection::send(const Packet &packet)
{
	/// @validate Connection is open.
	if (!this->isOpen())
	{
		NLOG ("Connection closed.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return false;
	}

	/// @validate Valid value required for remote port.
	if (this->_remote_port == 0)
	{
		NLOG ("Invalid port.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return false;
	}

	/// @validate Network address of message recipient
	if (this->_remote_address.to_string().empty())
	{
		// Assume broadcast address if one is not explicitly provided
		this->setRemoteAddress(RSSD::Network::BROADCAST_ADDRESS.to_string());
	}

	/// @validate Endpoint exists.
	if (this->_remote_endpoint == NULL)
	{
		NLOG ("Invalid endpoint.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return false;
	}

	// Queue the packet to send
	this->_sendq.push(packet);

	// Send packet
	this->_service->post(
		boost::bind(
			&Connection::onSend,
			this->getPointer(),
			boost::cref(packet),
			this->_remote_endpoint));
	return true;
}

bool UDP::Connection::broadcast(const Packet &packet)
{
	/// @validate Connection is open.
	if (!this->isOpen())
	{
		NLOG ("Connection closed.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return false;
	}

	/// @validate Valid value required for broadcast port.
	if (!this->_broadcast_port)
	{
		NLOG ("Invalid port.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return false;
	}

	/// @validate Network address of message recipient
	if (this->_broadcast_address.to_string().empty())
	{
		// Assume broadcast address if one is not explicitly provided
		this->setBroadcastAddress(RSSD::Network::BROADCAST_ADDRESS.to_string());
	}

	/// @validate Endpoint exists
	if (this->_broadcast_endpoint == NULL)
	{
		NLOG ("Invalid endpoint.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return false;
	}

	// Queue the sent packet
	this->_sendq.push(packet);

	NLOG ("Sending packet: " << packet, this->getLogGroup().c_str(), Log::Level::INSANE);
	this->_service->post(
		boost::bind(
			&Connection::onSend,
			this->getPointer(),
			boost::cref(packet),
			this->_broadcast_endpoint));
	return true;
}

void UDP::Connection::onSend(
	const Packet &packet,
	const udp_endpoint_ptr endpoint)
{
	/// @validate Connection is open.
	if (!this->isOpen())
	{
		NLOG ("Connection closed.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return;
	}

	// Send packet
	this->_socket.async_send_to(
		bnet::buffer(
			this->_sendq.back().toBytes(),
			this->_sendq.back().getSize()),
		*endpoint,
		boost::bind(
			&Connection::onWrite,
			this->getPointer(),
			boost::cref(packet),
			bnet::placeholders::bytes_transferred,
			bnet::placeholders::error));
}

void UDP::Connection::onWrite(
	const Packet &packet,
	const uint32_t bytes_transferred,
	const boost::system::error_code &error)
{
	/// @validate Transmission success.
	if (error)
	{
		NLOG (error.message(), this->getLogGroup().c_str(), Log::Level::WARNING);
		return;
	}

	// Update listeners with sent packet
	/// @todo Implement using std::for_each(...)
	ConnectionListener_l::iterator
		iter = this->_listener_manager.getItems().begin(),
		end = this->_listener_manager.getItems().end();
	for (; iter != end; ++iter)
	{
		ConnectionListener listener = *iter;
		listener->onPacketSent(
			this->shared_from_this(),
			this->_sendq.front());
	}

	// De-queue the sent packet
	this->_sendq.pop();
}

bool UDP::Connection::receive()
{
	/// @validate Connection must be open.
	if (!this->isOpen())
	{
		NLOG ("Connection closed.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return false;
	}

	this->_service->post(
		boost::bind(
			&Connection::onReceive,
			this->getPointer()));
	return true;
}

///
/// @todo Determine if the underlying packet buffer
/// retains its size after a ::clear() command,
/// which would invalidate the need to resize
/// the buffer here on each ::receive() call.
///

void UDP::Connection::onReceive()
{
	/// @validate Connection must be open.
	if (!this->isOpen())
	{
		NLOG ("Connection closed.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return;
	}

	// Clear stale packet data and resize packet buffer
	this->_recvq.push(byte_v(Packet::MAX_UDP_SIZE));

	// Read packet off the wire
	this->_socket.async_receive_from(
		bnet::buffer(
			this->_recvq.back(),
			Packet::MAX_UDP_SIZE),
		this->_remote_endpoint_placeholder,
		boost::bind(
			&Connection::onReceiveDatagram,
			this->getPointer(),
			bnet::placeholders::bytes_transferred,
			bnet::placeholders::error));
}

void UDP::Connection::onReceiveDatagram(
	const uint32_t bytes_transferred,
	const boost::system::error_code &error)
{
	/// @validate Receipt success.
	if (error)
	{
		NLOG (error.message(), this->getLogGroup().c_str(), Log::Level::WARNING);
		return;
	}

	// Create packet from received bytes
	byte_v &bytes = this->_recvq.back();
	Packet packet(&bytes[0]);

	// Update listeners with new packet
	/// @todo Implement using std::for_each(...)
	ConnectionListener_l::iterator
		iter = this->_listener_manager.getItems().begin(),
		end = this->_listener_manager.getItems().end();
	for (; iter != end; ++iter)
	{
		ConnectionListener listener = *iter;
		listener->onPacketReceived(
			UDP::Connection::shared_from_this(),
			packet);
	}

	// De-queue the received packet
	/// @hack Figure out how to truly utilize a received packet queue...
	this->_recvq.pop();
	assert (this->_recvq.empty());

	// Listen for next UDP packet
	this->receive();
}
