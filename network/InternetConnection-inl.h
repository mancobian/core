template <typename PROTOCOL>
InternetConnection<PROTOCOL>::InternetConnection() :
	Connection(),
	_listen_endpoint(new Endpoint(Network::ANY_ADDRESS, 0)),
	_remote_endpoint(new Endpoint(Network::ANY_ADDRESS, 0)),
	_socket(*this->_service)
{
	this->init();
}

template <typename PROTOCOL>
InternetConnection<PROTOCOL>::InternetConnection(
	io_service_ptr service) :
	Connection(service),
	_listen_endpoint(new Endpoint(Network::ANY_ADDRESS, 0)),
	_remote_endpoint(new Endpoint(Network::ANY_ADDRESS, 0)),
	_socket(*service)
{
	this->init();
}

template <typename PROTOCOL>
InternetConnection<PROTOCOL>::~InternetConnection()
{
    NLOG ("Connection destroyed.", this->getLogGroup().c_str(), Log::Level::DEBUG);
}

template <typename PROTOCOL>
void InternetConnection<PROTOCOL>::init()
{
	// Initialize connection protocol
	this->_protocol = this->_listen_endpoint->protocol().type();

	// Initialize connection ID (and log group)
	this->setId(0);

	// Initialize endpoint values
	this->updateEndpoints();
}

template <typename PROTOCOL>
std::string InternetConnection<PROTOCOL>::getEndpointsAsString() const
{
	std::stringstream output;
	output << "Connection endpoints: ";
	if (this->_listen_endpoint)
		output << "(Listen) " << this->_listen_endpoint << " ";
	if (this->_remote_endpoint)
		output << "(Remote) " << this->_remote_endpoint << " ";
	return output.str();
}

template <typename PROTOCOL>
bool InternetConnection<PROTOCOL>::updateEndpoints()
{
    NLOG ("Updating endpoints...", this->getLogGroup().c_str(), Log::Level::INSANE);
	bool result = this->updateListenEndpoint();
	result |= this->updateRemoteEndpoint();
	return result;
}

template <typename PROTOCOL>
bool InternetConnection<PROTOCOL>::updateListenEndpoint()
{
	/// @validate Non-zero value required for socket port.
	if (!this->_listen_port)
	{
        NLOG ("Invalid port.", this->getLogGroup().c_str(), Log::Level::INSANE);
		return false;
	}

	this->_listen_endpoint.reset(
		new Endpoint(
			RSSD::Network::ANY_ADDRESS,
			this->_listen_port));
	return true;
}

template <typename PROTOCOL>
bool InternetConnection<PROTOCOL>::updateRemoteEndpoint()
{
	/// @validate Non-zero value required for socket port.
	if (!this->_remote_port)
	{
        NLOG ("Invalid port.", this->getLogGroup().c_str(), Log::Level::INSANE);
		return false;
	}

	/// @validate Non-empty value required for socket address.
	if (this->_remote_address.to_string().empty())
	{
        NLOG ("Invalid IP address.", this->getLogGroup().c_str(), Log::Level::INSANE);
		return false;
	}

	this->_remote_endpoint.reset(
		new Endpoint(
			this->_remote_address,
			this->_remote_port));
	return true;
}

template <typename PROTOCOL>
bool InternetConnection<PROTOCOL>::setSocketOptions()
{
	/// @validate Connection open.
	if (!this->isOpen())
	{
        NLOG ("Connection closed.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return false;
	}

	// Configure socket options
	this->_socket.set_option(typename Socket::linger(false, 0));
	this->_socket.set_option(typename Socket::keep_alive(true));
	this->_socket.set_option(typename Socket::reuse_address(true));
	// this->_socket.set_option(Socket::non_blocking_io(true));
	// this->_socket.set_option(bnet::ip::multicast::join_group(this->_broadcast_address)); // @note Join the multicast group
	// this->_socket.set_option(bnet::ip::tcp::socket::broadcast(false));
	// this->_socket.set_option(Socket::non_blocking_io(true));
	// this->_socket.set_option(Socket::receive_buffer_size(Packet::MAX_UDP_SIZE));
	// this->_socket.set_option(Socket::send_buffer_size(Packet::MAX_UDP_SIZE));
	return true;
}

template <typename PROTOCOL>
bool InternetConnection<PROTOCOL>::open()
{
	return this->open(false);
}

template <typename PROTOCOL>
bool InternetConnection<PROTOCOL>::open(const bool is_datagram)
{
	// Invoke base class method
	if (!Connection::open())
	{
        NLOG ("Failed to open connection.", this->getLogGroup().c_str(), Log::Log::Level::ERROR);
		return false;
	}

	/// @validate Valid endpoint(s) for socket binding.
    NLOG ("Updating connection endpoints...", this->getLogGroup().c_str(), Log::Level::INSANE);
	this->updateEndpoints();
    NLOG ("" << this->getEndpointsAsString(), this->getLogGroup().c_str(), Log::Level::DEBUG);

    NLOG ("Opening connection. Is datagram? " << is_datagram, this->getLogGroup().c_str(), Log::Level::INSANE);
	this->_service->post(
		boost::bind(
			&InternetConnection<PROTOCOL>::onOpen,
			this->getPointer(),
			is_datagram));
	return true;
}

template <typename PROTOCOL>
bool InternetConnection<PROTOCOL>::onOpen(const bool is_datagram)
{
	/// @validate Connection closed.
    NLOG ("On opening connection...", this->getLogGroup().c_str(), Log::Level::INSANE);
	if (this->isOpen())
	{
        NLOG ("Connection open.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return false;
	}

	// Bind to local socket endpoint
	boost::system::error_code error;
	if (this->_listen_endpoint)
	{
		// Open socket
        NLOG ("Opening socket...", this->getLogGroup().c_str(), Log::Level::INSANE);
		this->_socket.open(this->_listen_endpoint->protocol(), error);
		if (error)
		{
            NLOG ("Failed to open socket <endpoint=" << this->_listen_endpoint << ">.", this->getLogGroup().c_str(), Log::Log::Level::ERROR);
			return false;
		}

		// Configure socket options
        NLOG ("Setting socket options...", this->getLogGroup().c_str(), Log::Level::INSANE);
		assert (this->setSocketOptions());

		// Bind socket
        NLOG ("Binding socket to endpoint: " << this->_listen_endpoint, this->getLogGroup().c_str(), Log::Level::INSANE);
		this->_socket.bind(*this->_listen_endpoint, error);
		if (error)
		{
            NLOG ("Failed to bind socket <endpoint=" << this->_listen_endpoint << ">.", this->getLogGroup().c_str(), Log::Log::Level::ERROR);
			return false;
		}
	}

	// Connect to a remote socket endpoint
	if (!is_datagram && this->_remote_endpoint)
	{
		// Prepare TCP connection data
		std::stringstream portstring;
		portstring << this->_remote_port;
		typename InternetConnection::Resolver resolver(*this->_service);
		typename InternetConnection::Resolver::query query(this->_remote_address.to_string(), portstring.str());
		typename InternetConnection::Resolver::iterator endpoint_iterator = resolver.resolve(query);

		/// @validate Successfully resolved the given connection endpoint.
		if (endpoint_iterator == typename Resolver::iterator())
		{
            NLOG ("Failed to connect to endpoint <" << this->_remote_endpoint << ">", this->getLogGroup().c_str(), Log::Log::Level::ERROR);
			return false;
		}

		// Wait for connection
        NLOG ("Beginning async connect(...) for connection <this=" << this->getId() << ", endpoint=" << this->_remote_endpoint << ">", this->getLogGroup().c_str(), Log::Level::DEBUG);
		Endpoint endpoint = *endpoint_iterator;
		this->_socket.async_connect(
			endpoint,
			boost::bind(
				&InternetConnection<PROTOCOL>::onConnect,
				this->getPointer(),
				++endpoint_iterator,
				bnet::placeholders::error));
	}
	else
	{
		// Begin connection read loop
        NLOG ("Beginning receive(...) loop...", this->getLogGroup().c_str(), Log::Level::INSANE);
		this->receive();
	}

	// Update listeners on connection event
	/// @todo Implement using std::for_each(...)
	ConnectionListener_l::iterator
		iter = this->_listener_manager.getItems().begin(),
		end = this->_listener_manager.getItems().end();
	for (; iter != end; ++iter)
	{
		ConnectionListener listener = *iter;
		this->_service->post(
			boost::bind(
				&Connection::Listener::onConnectionOpened,
				listener,
				this->shared_from_this()));
	}
	return true;
}

template <typename PROTOCOL>
void InternetConnection<PROTOCOL>::onConnect(
	typename Resolver::iterator endpoint_iterator,
	const boost::system::error_code &error)
{
    endpoint_iterator = endpoint_iterator;

	/// @validate Successfully established connection to remote socket endpoint.
	if (error)
	{
		/// @todo Attempt to re-establish connection on failure.
        NLOG ("" << error.message(), this->getLogGroup().c_str(), Log::Log::Level::ERROR);
		this->close();
		return;
	}

	// Begin connection read loop
    NLOG ("Connected!", this->getLogGroup().c_str(), Log::Level::DEBUG);
	this->receive();
}

template <typename PROTOCOL>
bool InternetConnection<PROTOCOL>::close()
{
	/// @validate Connection is open.
	if (!this->isOpen())
	{
        NLOG ("Connection closed.", this->getLogGroup().c_str(), Log::Level::WARNING);
		return false;
	}

	// Close socket
	try
	{
		boost::system::error_code error;
		this->_socket.shutdown(Socket::shutdown_both);
		this->_socket.cancel(error);
		this->_socket.close();
	}
	catch (const std::exception &e)
	{
        NLOG (e.what(), this->getLogGroup().c_str(), Log::Log::Level::ERROR);
		return false;
	}

	// Update listeners on connection event
	/// @todo Implement using std::for_each(...)
    NLOG ("Closing connection; updating <" << this->_listener_manager.getItems().size() << "> listeners", this->getLogGroup().c_str(), Log::Level::DEBUG);
	ConnectionListener_l::iterator
		iter = this->_listener_manager.getItems().begin(),
		end = this->_listener_manager.getItems().end();
	while (iter != end)
	{
		ConnectionListener listener = *iter++;
		listener->onConnectionClosed(this->shared_from_this());
	}

	// Invoke base class method
	return Connection::close();
}
