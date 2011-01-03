///
/// @file BinarySerializer.h
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

#ifndef RSSD_BINARYSERIALIZER_H
#define RSSD_BINARYSERIALIZER_H

#include "System"
#include "serialization/Serializer.h"

namespace RSSD {

class BinarySerializer : public Serializer<byte*>
{
public:
  BinarySerializer(const DataMemberMap &dataMembers);
  virtual ~BinarySerializer();
  virtual uint_t to(byte *&dst) const;
  virtual uint_t from(byte *&src);
}; /// class BinarySerializer

template <typename T>
class BinarySerializable : public BinarySerializer::Serializable
{
public:
  BinarySerializable();
  BinarySerializable(const T &value);
  virtual ~BinarySerializable();
  virtual uint_t size() const;
  virtual uint_t to(byte *&dst) const;
  virtual uint_t from(byte *&src);
  virtual T& get() { return this->mValue; }
  virtual void set(const T &value) { this->mValue = value; }

protected:
  const static uint_t SIZE;
  T mValue;
}; /// class BinarySerializable

///
/// Typedefs
///

typedef BinarySerializable<char> bchar_t;
typedef BinarySerializable<short> bshort_t;
typedef BinarySerializable<int> bint_t;
typedef BinarySerializable<float> bfloat_t;
typedef BinarySerializable<double> bdouble_t;

///
/// Includes
///

#include "BinarySerializer-inl.h"

} /// namespace RSSD

#endif // RSSD_BINARYSERIALIZER_H
