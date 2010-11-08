#include "Common.h"

using namespace RSSD;
using namespace RSSD::Network;

const bnet::ip::address RSSD::Network::LOOPBACK_ADDRESS = bnet::ip::address_v4::loopback();
const bnet::ip::address RSSD::Network::ANY_ADDRESS = bnet::ip::address_v4::any();
const bnet::ip::address RSSD::Network::BROADCAST_ADDRESS = bnet::ip::address_v4::broadcast();

bool RSSD::Network::randbool()
{
	static bool INIT = false;
	if (!INIT)
	{
		INIT = true;
		std::srand(std::time(NULL));
	}

	uint32_t random = rand();
	float value = (float)random / (float)RAND_MAX;
	if (value < 0.5f)
	{
		// NLOG ("randbool(" << random << ", " << value << ")=false");
		return false;
	}
	// NLOG ("randbool(" << random << ", " << value << ")=true");
	return true;
}

void RSSD::Network::printBytes(
	const byte_v bytes,
	std::ostream &out)
{
	printBytes(
		&bytes[0],
		bytes.size(),
		out);
}

void RSSD::Network::printBytes(
	const byte *bytes,
	const uint32_t size,
	std::ostream &out)
{
	out << "[";
	for (uint32_t i=0; i<size; ++i)
	{
		out << (uint32_t)bytes[i];
		if (i < size-1)
			out << " ";
	}
	out << "] (" << size << " bytes)";
}

std::ostream& RSSD::Network::operator <<(
	std::ostream &out,
	const byte_v bytes)
{
	printBytes(bytes, out);
	return out;
}
