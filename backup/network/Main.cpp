#include <network/Common.h>
#include <network/BasicClient.h>
#include <network/BasicServer.h>
#include <network/test/BasicMessage.h>
#include <network/test/BasicListener.h>
#include <network/test/BasicClientManager.h>

using namespace RSSD;
using namespace RSSD::Network;

///
/// Global Variables
///

const uint32_t
	NUM_CLIENTS = 1,
	TCP_PORT = 10500,
	UDP_CLIENT_PORT = 9998,
	UDP_SERVER_PORT = 9999,
	UDP_BROADCAST_PORT = UDP_CLIENT_PORT;
const char
	*UDP_BROADCAST_ADDRESS = "192.168.0.255",
	*NLOG_GROUP_ID = "NET/MAIN";

///
/// Global Functions
///

void testMessageCreate()
{
	// Arbitrary message data
	uint32_t number = 7;
	string_t text = "Happy Time!";

	// Create message
	BasicMessage source_message(number, text);
	uint32_t size = source_message.getSize() + Message::Header::getSize();
	NLOG ("Created a message.", NLOG_GROUP_ID);
	NLOG ("\t=> Type: " << source_message.getType(), NLOG_GROUP_ID);
	NLOG ("\t=> Version: " << source_message.getVersion(), NLOG_GROUP_ID);
	NLOG ("\t=> Size: " << source_message.getSize(), NLOG_GROUP_ID);
	NLOG ("\t=> Values: " << source_message.getInt() << ", " << source_message.getString(), NLOG_GROUP_ID);

	NLOG ("Converted message to bytestream.", NLOG_GROUP_ID);
	const byte *bytes = source_message.toBytes();
	printBytes(bytes, size);

	NLOG ("Restored message from bytestream.", NLOG_GROUP_ID);
	BasicMessage copy_message;
	copy_message.fromBytes(bytes);
	NLOG ("\t=> Type: " << copy_message.getType(), NLOG_GROUP_ID);
	NLOG ("\t=> Version: " << copy_message.getVersion(), NLOG_GROUP_ID);
	NLOG ("\t=> Size: " << copy_message.getSize(), NLOG_GROUP_ID);
	NLOG ("\t=> Values: " << copy_message.getInt() << ", " << copy_message.getString(), NLOG_GROUP_ID);
}

void testManagedClients(
	const bool is_sleep_on_send = true,
	const uint32_t seconds = 5)
{
	// Register message factory(s)
	Message::FactoryManager::getPointer()->registerFactory(new BasicMessage::Factory());

	// Local vars
	NLOG ("Creating local vars...", NLOG_GROUP_ID);
	BasicListener::Pointer listener(new BasicListener());
	BasicClientManager::Pointer client_manager(
		new BasicClientManager(
			TCP_PORT,
			UDP_CLIENT_PORT,
			UDP_SERVER_PORT,
			UDP_BROADCAST_ADDRESS,
			UDP_BROADCAST_PORT,
			NUM_CLIENTS));
	BasicServer::Pointer server(
		new BasicServer(
			TCP_PORT,
			UDP_SERVER_PORT,
			UDP_BROADCAST_PORT));

	// Configure application-specific settings
	BasicClient::APPLICATION_ID = 1;

	// Configure and start server
	NLOG ("Starting server...", NLOG_GROUP_ID);
	server->addListener(listener);
	server->start();

	// Create clients
	NLOG ("Creating clients...", NLOG_GROUP_ID);
	client_manager->start();

	// Test client packet transmission
	NLOG ("Sending packets...", NLOG_GROUP_ID);
	client_manager->send();

	/// @note Big sleep to allow server to receive packet(s)
	if (is_sleep_on_send)
		sleep (seconds);

	// Destroy clients
	NLOG ("Destroying clients...", NLOG_GROUP_ID);
	client_manager->stop();

	// Stop server
	NLOG ("Stopping server...", NLOG_GROUP_ID);
	server->stop();

	// Destroy local vars
	NLOG ("Destroying local vars...", NLOG_GROUP_ID);
	listener.reset();
	client_manager.reset();
	server.reset();
}

///
/// Entry Point
///

int main(int argc, char **argv)
{
	// Create singletons
	new RSSD::LogManager();
	new Message::FactoryManager();

	// Configure logger
    RSSD::LogManager::getPointer()->setLogLevel(Log::Level::INSANE);

	// Test: Managed clients
	testManagedClients();

	// Test: Message creation
	// testMessageCreate();

	/// @hack Sleep to prevent issues with destroying LogManager
	/// before network objects' destruction routines complete, some
	/// of which use the LogManager.
	/// @todo Implement a message queue for LogManager (?); it currently
	/// synchronizes (locks) on EACH output message.
    sleep (1);
	NLOG ("Done!", NLOG_GROUP_ID);

	// Destroy singletons
    delete Message::FactoryManager::getPointer();
	delete RSSD::LogManager::getPointer();
	return 0;
}
