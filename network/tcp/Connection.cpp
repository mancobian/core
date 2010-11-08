#include "Connection.h"
#include <network/Packet.h>

using namespace RSSD;
using namespace RSSD::Network;

const bnet::ip::tcp::resolver::iterator TCP::Connection::RESOLVE_END;

TCP::Connection::Connection(
	io_service_ptr service) :
	BaseConnection(service)
{
}

TCP::Connection::Connection(
	io_service_ptr service,
	const std::string &host,
	const unsigned short port) :
	BaseConnection(service)
{
}

TCP::Connection::~Connection()
{
	this->close();
}

bool TCP::Connection::send(const Packet &packet)
{
	if (!this->isOpen())
	{
		NLOG ("Connection closed.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return false;
	}

	// Queue the sent packet
	this->_sendq.push(packet);
	this->_service->post(
		boost::bind(
			&Connection::onSend,
			this->getPointer(),
			boost::cref(this->_sendq.back())));
	return true;
}

void TCP::Connection::onSend(const Packet &packet)
{
	/// @validate Connection is open.
	if (!this->isOpen())
	{
		NLOG ("Connection closed.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return;
	}

	bnet::async_write(
		this->_socket,
		bnet::buffer(
			this->_sendq.back().toBytes(),
			this->_sendq.back().getSize()),
		boost::bind(
			&Connection::onWrite,
			this->getPointer(),
			boost::cref(this->_sendq.back()),
			bnet::placeholders::bytes_transferred,
			bnet::placeholders::error));
}

void TCP::Connection::onWrite(
	const Packet &packet,
	const uint32_t bytes_transferred,
	const boost::system::error_code &error)
{
	/// @validate Successful network transmission.
	if (error)
	{
		NLOG (error.message(), this->getLogGroup().c_str(), Log::Level::ERROR);
		return;
	}

	NLOG ("Sending packet (" << packet.getSize() << " bytes) to endpoint=" << this->_remote_endpoint, this->getLogGroup().c_str(), Log::Level::DEBUG);
	assert (packet == this->_sendq.front());

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

bool TCP::Connection::receive()
{
	/// @validate Connection is open.
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
void TCP::Connection::onReceive()
{
	/// @validate Connection is open.
	if (!this->isOpen())
	{
		NLOG ("Connection closed.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return;
	}

	// Create a new packet to receive
	this->_recvq.push(byte_v(Packet::Header::getSize()));

	// Read packet header off the wire
	bnet::async_read(
		this->_socket,
		bnet::buffer(
			this->_recvq.back(),
			Packet::Header::getSize()),
		boost::bind(
			&Connection::onReceiveHeader,
			this->getPointer(),
			bnet::placeholders::bytes_transferred,
			bnet::placeholders::error));
}

void TCP::Connection::onReceiveHeader(
	const uint32_t bytes_transferred,
	const boost::system::error_code &error)
{
	/// @validate Successful packet header reception.
	if (error)
	{
		NLOG (error.message(), this->getLogGroup().c_str(), Log::Level::ERROR);
		return;
	}

	// Resize packet buffer
	byte_v &bytes = this->_recvq.back();
	NLOG ("Received header: " << bytes, this->getLogGroup().c_str(), Log::Level::INSANE);
	Packet::Header header = Packet::Header::parse(&bytes[0]);
	bytes.resize(Packet::Header::getSize() + header.Size);

	// Read packet payload off the wire
	bnet::async_read(
		this->_socket,
		bnet::buffer(
			&bytes[Packet::Header::getSize()], // @note Offset buffer write index
			header.Size),
		boost::bind(
			&Connection::onReceivePayload,
			this->getPointer(),
			bnet::placeholders::bytes_transferred,
			bnet::placeholders::error));
}

void TCP::Connection::onReceivePayload(
	const uint32_t bytes_transferred,
	const boost::system::error_code &error)
{
	/// @validate Successful packet payload reception.
	if (error)
	{
		NLOG (error.message(), this->getLogGroup().c_str(), Log::Level::ERROR);
		return;
	}

	// Create packet
	byte_v &bytes = this->_recvq.back();
	Packet packet(&bytes[0]);
	NLOG ("Received packet: " << packet, this->getLogGroup().c_str(), Log::Level::INSANE);

	// Update listeners with new packet
	/// @todo Implement using std::for_each(...)
	ConnectionListener_l::iterator
		iter = this->_listener_manager.getItems().begin(),
		end = this->_listener_manager.getItems().end();
	for (; iter != end; ++iter)
	{
		ConnectionListener listener = *iter;
		NLOG ("Updating connection listener <this=" << listener.get() << "> on packet received.", this->getLogGroup().c_str(), Log::Level::DEBUG);
		listener->onPacketReceived(
			this->shared_from_this(),
			packet);
	}

	// De-queue the received packet
	// @hack Figure out how to truly utilize a received packet queue...
	this->_recvq.pop();
	assert (this->_recvq.empty());

	// Read next available packet off the wire
	this->receive();
}
