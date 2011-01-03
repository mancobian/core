///
/// @file Common.h
/// @author Mancobian Poemandres
/// @license BSD License
///
/// Copyright (c) MMX by The Secret Design Collective
/// All rights reserved
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
///    * Redistributions of source code must retain the above copyright notice,
/// 		this list of conditions and the following disclaimer.
///    * Redistributions in binary form must reproduce the above copyright notice,
/// 		this list of conditions and the following disclaimer in the documentation
/// 		and/or other materials provided with the distribution.
///    * Neither the name of The Secret Design Collective nor the names of its
/// 		contributors may be used to endorse or promote products derived from
/// 		this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
/// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///

#ifndef NOUS_NETWORK_COMMON_H
#define NOUS_NETWORK_COMMON_H

#include <cstdlib>
#include <ctime>
#include <climits>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include "Core"
#include "Pattern"
#include "Log"
#include "Types.h"

namespace RSSD {
namespace Network {

///
/// Global Variables
///

extern const bnet::ip::address LOOPBACK_ADDRESS;
extern const bnet::ip::address ANY_ADDRESS;
extern const bnet::ip::address BROADCAST_ADDRESS;

///
/// Global Functions
///

bool randbool();
void printBytes(
	const byte_v bytes,
	std::ostream &out = std::cout);
void printBytes(
	const byte *bytes,
	const uint32_t size,
	std::ostream &out = std::cout);

template <typename PROTOCOL>
void print(
	boost::shared_ptr<boost::asio::ip::basic_endpoint<PROTOCOL> > endpoint,
	std::ostream &out = std::cout)
{
	std::stringstream endpoint_s;
	endpoint_s << endpoint->address().to_string()
		<< ":"
		<< endpoint->port();
	out << endpoint_s.str();
}

///
/// Global Functions
///

std::ostream& operator <<(
	std::ostream &out,
	const byte_v bytes);

template <typename PROTOCOL>
std::ostream& operator <<(
	std::ostream &out,
	boost::shared_ptr<boost::asio::ip::basic_endpoint<PROTOCOL> > endpoint)
{
	print(endpoint, out);
	return out;
}

} // namespace Network
} // namespace RSSD

#endif // NOUS_NETWORK_COMMON_H

