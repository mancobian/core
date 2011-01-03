#ifndef NOUS_NETWORKING_BASICLISTENER_HPP
#define NOUS_NETWORKING_BASICLISTENER_HPP

#include <network/Common.h>
#include <network/Connection.h>
#include <network/Packet.h>
#include <network/test/BasicMessage.h>

namespace RSSD {
namespace Network {

///
/// @struct BasicListener
///

struct BasicListener :
	public boost::enable_shared_from_this<BasicListener>,
	public RSSD::Network::Connection::Listener
{
public:
	typedef boost::shared_ptr<BasicListener> Pointer;

public:
	static const uint32_t APPLICATION_ID = 1;

public:
	virtual void onConnectionOpened(RSSD::Network::Connection::Pointer connection)
	{
		NLOG ("Connection <" << connection->getId() << "> opened.", "NET/BASICLISTENER");
	}

	virtual void onConnectionClosed(RSSD::Network::Connection::Pointer connection)
	{
		NLOG ("Connection <" << connection->getId() << "> closed.", "NET/BASICLISTENER");
	}

	virtual void onPacketReceived(Connection::Pointer connection, const RSSD::Network::Packet &packet)
	{
		BasicMessage message;
		message.fromBytes(packet.getPayload());
		NLOG ("Received message from <" << connection->getId() << ">: " << message << ".", "NET/BASICLISTENER");
		NLOG ("\t=> Message (int): " << message.getInt(), "NET/BASICLISTENER");
		NLOG ("\t=> Message (string): " << message.getString(), "NET/BASICLISTENER");
	}

	virtual void onPacketSent(Connection::Pointer connection, const RSSD::Network::Packet &packet)
	{
		NLOG ("Sent packet from <" << connection->getId() << ">: " << packet, "NET/BASICLISTENER");
	}
}; // struct BasicListener

} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORKING_BASICLISTENER_HPP
