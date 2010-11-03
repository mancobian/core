///
/// @file HashedString.h
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

#ifndef RSSD_CORE_SYSTEM_STRID_H
#define RSSD_CORE_SYSTEM_STRID_H

#include "Type.h"
#include "ThirdParty.h"

namespace rssd {
namespace system {

/// @note 'id' is a keyword in some languages.
/// @todo Consider alternatives to 'id' as a variable name.
class strid
{
public:
	typedef std::map<std::string, uint32_t> strid_m;
	typedef std::set<uint32_t> id_s;

public:
    strid() :
        _id(0),
        _text("__uninitialized__")
    {
    }

    strid(const char *text) :
        _id(0),
        _text(text)
    {
        this->_id = get_hash(this->_text);
    }

    strid(uint32_t id, const char *text) :
        _id(id),
        _text(text)
    {
    }

    strid(const strid &rhs) :
        _id(rhs._id),
        _text(rhs._text)
    {
    }

    ~strid()
    {
    }

public:
    strid& operator =(const char *value)
    {
        this->_text.assign(value);
        this->_id = strid::get_hash(this->_text);
        return *this;
    }

    bool operator <(const char *value) const
    {
        return (this->_id < strid::get_hash(value));
    }

    bool operator ==(const char *value) const
    {
        return (this->_id == strid::get_hash(value));
    }

    bool operator <(const uint32_t value) const
    {
        return (this->_id < value);
    }

    bool operator ==(const uint32_t value) const
    {
        return (this->_id == value);
    }

    strid& operator =(const uint32_t value)
    {
        this->_id = value;
        return *this;
    }

    bool operator <(const strid &value) const
    {
        return (this->_id < value._id);
    }

    bool operator ==(const strid &value) const
    {
        return (this->_id == value._id);
    }

    strid& operator =(const strid &value)
    {
        if (this != &value)
        {
            this->_id = value._id;
            this->_text = value._text;
        }
        return *this;
    }

public:
    static uint32_t get_hash(const std::string &text)
    {
        // Concurrency lock
        static boost::mutex MUTEX;
        boost::mutex::scoped_lock lock(MUTEX);

        // Return ID if input string has already been hashed
        if (strid::HASHMAP.find(text) != HASHMAP.end())
            return HASHMAP[text];

        // Create string hash
        std::locale c_locale; // the "C" locale
        const std::collate<char>& collate_char = std::use_facet<std::collate<char> >(c_locale);
        uint32_t id = collate_char.hash(text.data(), text.data() + text.length());
        assert (strid::HASHSET.find(id) == HASHSET.end());

        // Store string hash
        HASHMAP.insert(std::make_pair(text, id));
        HASHSET.insert(id);
        return id;
    }

protected:
	static id_s HASHSET;
	static strid_m HASHMAP;

protected:
	uint32_t _id;
	std::string _text;
}; // class strid

} // namespace system
} // namespace rssd

///
/// Macros
///

#define STRID(VALUE) rssd::system::strid::get_hash(#VALUE);

#endif // RSSD_CORE_SYSTEM_STRID_H
