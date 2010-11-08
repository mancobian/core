#include "BasicClient.h"

using namespace RSSD;
using namespace RSSD::Network;

uint32_t RSSD::Network::BasicClient::APPLICATION_ID = 0;

BasicClient::BasicClient() :
	Connection(),
	_tcp_client(new TCP::Client(this->_service)),
	_udp_client(new UDP::Client(this->_service))
{
}

BasicClient::BasicClient(io_service_ptr service) :
	Connection(service),
	_tcp_client(new TCP::Client(service)),
	_udp_client(new UDP::Client(service))
{
}

BasicClient::~BasicClient()
{
	this->close();
}

bool BasicClient::open()
{
	boost::mutex::scoped_lock lock(this->_mutex);
	bool result = this->_tcp_client->open();
	result &= this->_udp_client->open();
	return Connection::open();
}

bool BasicClient::close()
{
	// boost::mutex::scoped_lock lock(this->_mutex);
	bool result = this->_tcp_client->close();
	result &= this->_udp_client->close();
	return Connection::close();
}

bool BasicClient::receive()
{
	return true;
}

bool BasicClient::send(
	const Message &message,
	const bool is_critical)
{
	return this->send(
		Packet(BasicClient::APPLICATION_ID, &message),
		is_critical);
}

bool BasicClient::send(const Packet &packet)
{
	return this->send(packet, true);
}

bool BasicClient::send(
	const Packet &packet,
	const bool is_critical)
{
	if (!is_critical)
		return this->_udp_client->send(packet);
	return this->_tcp_client->send(packet);
}

bool BasicClient::broadcast(const Packet &packet)
{
	return this->_udp_client->broadcast(packet);
}

bool BasicClient::hasListener(Connection::Listener::Pointer listener)
{
	bool result = this->_tcp_client->hasListener(listener);
	result &= this->_udp_client->hasListener(listener);
	return result;
}

bool BasicClient::addListener(Connection::Listener::Pointer listener)
{
	bool result = this->_tcp_client->addListener(listener);
	result &= this->_udp_client->addListener(listener);
	return result;
}

bool BasicClient::removeListener(Connection::Listener::Pointer listener)
{
	bool result = this->_tcp_client->removeListener(listener);
	result &= this->_udp_client->removeListener(listener);
	return result;
}

void BasicClient::clearListeners()
{
	this->_tcp_client->clearListeners();
	this->_udp_client->clearListeners();
}
