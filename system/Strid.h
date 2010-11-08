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
#include "Concurrency.h"
#include "ThirdParty.h"

namespace RSSD {

/// @note 'id' is a keyword in some languages.
/// @todo Consider alternatives to 'id' as a variable name.
class Strid
{
public:
  typedef std::map<std::string, uint32_t> Strid_m;
  typedef std::set<uint32_t> id_s;

public:
  Strid();
  Strid(const char *text);
  Strid(uint32_t id, const char *text);
  Strid(const Strid &rhs);
  ~Strid();
  static uint32_t getHash(const std::string &text);
  uint32_t getId() const { return this->_id; }
  std::string getText() const { return this->_text; }

  inline Strid& operator =(const char *value)
  {
    this->_text.assign(value);
    this->_id = Strid::getHash(this->_text);
    return *this;
  }

  inline bool operator <(const char *value) const
  {
    return (this->_id < Strid::getHash(value));
  }

  inline bool operator ==(const char *value) const
  {
    return (this->_id == Strid::getHash(value));
  }

  inline bool operator <(const uint32_t value) const
  {
    return (this->_id < value);
  }

  inline bool operator ==(const uint32_t value) const
  {
    return (this->_id == value);
  }

  inline Strid& operator =(const uint32_t value)
  {
    this->_id = value;
    return *this;
  }

  inline bool operator <(const Strid &value) const
  {
    return (this->_id < value._id);
  }

  inline bool operator ==(const Strid &value) const
  {
    return (this->_id == value._id);
  }

  inline Strid& operator =(const Strid &value)
  {
    if (this != &value)
    {
      this->_id = value._id;
      this->_text = value._text;
    }
    return *this;
  }

protected:
  static boost::mutex HASHMUTEX;
  static id_s HASHSET;
  static Strid_m HASHMAP;
  uint32_t _id;
  std::string _text;
}; // class Strid

} // namespace RSSD

#endif // RSSD_CORE_SYSTEM_STRID_H
