#include "BasicServer.h"
#include <network/Connection.h>

using namespace RSSD;
using namespace RSSD::Network;

BasicServer::BasicServer(
	const uint16_t tcp_listen_port,
	const uint16_t udp_listen_port,
	const uint16_t udp_broadcast_port,
	const string_t &log_group) :
	Server(log_group),
	_tcp_server(new TCP::Server(this->_service, tcp_listen_port)),
	_udp_server(new UDP::Server(this->_service, udp_listen_port, udp_broadcast_port))
{

}

BasicServer::~BasicServer()
{
	this->stop();
}

bool BasicServer::start()
{
	bool result = true;
	if (!Server::start(true))
	{
        NLOG ("Failed to start basic server.", BasicServer::getLogGroup().c_str(), Log::Log::Level::ERROR);
		result = false;
	}
	if (!this->_tcp_server->start())
	{
        NLOG ("Failed to start TCP server.", BasicServer::getLogGroup().c_str(), Log::Log::Level::ERROR);
		result = false;
	}
	if (!this->_udp_server->start())
	{
        NLOG ("Failed to start UDP server.", BasicServer::getLogGroup().c_str(), Log::Log::Level::ERROR);
		result = false;
	}
	return result;
}

bool BasicServer::stop()
{
	bool result = true;
	if (!this->_tcp_server->stop())
	{
        NLOG ("Failed to stop TCP server.", BasicServer::getLogGroup().c_str(), Log::Log::Level::ERROR);
		result = false;
	}
	if (!this->_udp_server->stop())
	{
        NLOG ("Failed to stop UDP server.", BasicServer::getLogGroup().c_str(), Log::Log::Level::ERROR);
		result = false;
	}
	if (!Server::stop())
	{
        NLOG ("Failed to stop basic server.", BasicServer::getLogGroup().c_str(), Log::Log::Level::ERROR);
		result = false;
	}
	return result;
}

bool BasicServer::addListener(ConnectionListener item)
{
	bool result = this->_tcp_server->addListener(item);
	result &= this->_udp_server->addListener(item);
	return result;
}

bool BasicServer::removeListener(ConnectionListener item)
{
	bool result = this->_tcp_server->removeListener(item);
	result &= this->_udp_server->removeListener(item);
	return result;
}

void BasicServer::clearListeners()
{
	this->_tcp_server->clearListeners();
	this->_udp_server->clearListeners();
}
