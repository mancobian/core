///
/// @file UriScheme.h
/// @author Mancobian Poemandres
/// @license BSD License
///
/// Copyright (c) MMX by Royal Society of Secret Design
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
///    * Neither the name of Royal Society of Secret Design nor the names of its
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

#ifndef RSSD_CORE_FILESYSTEM_URISCHEME_H
#define RSSD_CORE_FILESYSTEM_URISCHEME_H

#include <boost/assign.hpp>
#include "../System.h"

namespace boost {
namespace filesystem {
namespace extended {

class UriScheme
{
public:
	enum Types
	{
		UNKNOWN = 0,
		ABOUT,
		FILE,
		FTP,
		HTTP,
		HTTPS,
		UDP
	}; // enum Schemes

public:
	typedef std::map<std::string, UriScheme::Types> SchemeMap;

public:
	static const char getTypeDelimiter() { return ':'; }

	static UriScheme::Types toInt(const std::string &type)
	{
		UriScheme::SchemeMap &scheme_map = UriScheme::getSchemeMap();
		UriScheme::SchemeMap::const_iterator iter = scheme_map.find(type);
		if (iter == scheme_map.end())
			return UriScheme::UNKNOWN;
		return iter->second;
	}

	static UriScheme::Types getType(const std::string &uri)
	{
		// Validate input as URI-type string
		std::string uri_copy(uri);
		size_t index = uri_copy.find_first_of(UriScheme::getTypeDelimiter());
		if (index == std::string::npos)
			return UriScheme::UNKNOWN;

		// Sanitize input string (in-place)
		boost::algorithm::to_lower(uri_copy);
		boost::algorithm::trim(uri_copy);

		// Parse scheme type
		std::string type = uri_copy.substr(0, index);

		// Convert to integer type
		return UriScheme::toInt(type);
	}

protected:
	static UriScheme::SchemeMap& getSchemeMap()
	{
		static UriScheme::SchemeMap SCHEME_MAP;
		if (!SCHEME_MAP.empty())
			return SCHEME_MAP;
		SCHEME_MAP.insert(std::make_pair("about", UriScheme::ABOUT));
		SCHEME_MAP.insert(std::make_pair("file", UriScheme::FILE));
		SCHEME_MAP.insert(std::make_pair("ftp", UriScheme::FTP));
		SCHEME_MAP.insert(std::make_pair("http", UriScheme::HTTP));
		SCHEME_MAP.insert(std::make_pair("https", UriScheme::HTTPS));
		SCHEME_MAP.insert(std::make_pair("udp", UriScheme::UDP));
		return SCHEME_MAP;
	}
}; // class UriScheme

} // namespace extended
} // namespace filesystem
} // namespace boost

#endif // TSDC_CORE_FILESYSTEM_UriScheme_H
