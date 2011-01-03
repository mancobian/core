#include "BinarySerializer.h"

using namespace RSSD;

///
/// @class BinarySerializer
///

BinarySerializer::BinarySerializer(const DataMemberMap &dataMembers) :
  Serializer<byte*>(dataMembers)
{
}

BinarySerializer::~BinarySerializer()
{
}

uint_t BinarySerializer::to(byte *&dst) const
{
  uint_t result = 0;
  byte *buffer = NULL;
  DataMemberMap::const_iterator
    iter = this->mDataMembers.begin(),
    end = this->mDataMembers.end();
  for (; iter != end; ++iter)
  {
    const Serializable *serializable = boost::any_cast<const Serializable*>(iter->second);
    buffer = &dst[result];
    result += serializable->to(buffer);
  }
  return result;
}

uint_t BinarySerializer::from(byte *&src)
{
  uint_t result = 0;
  byte *buffer = NULL;
  DataMemberMap::iterator
    iter = this->mDataMembers.begin(),
    end = this->mDataMembers.end();
  for (; iter != end; ++iter)
  {
    Serializable *serializable = boost::any_cast<Serializable*>(iter->second);
    buffer = &src[result];
    result += serializable->from(buffer);
  }
  return result;
}
#if 0
///
/// @class bstring
///

uint_t bstring::size() const
{
  return this->mValue.size();
}

uint_t bstring::to(byte *dst) const
{
  const uint_t size = this->size();
  this->mValue.copy(reinterpret_cast<char*>(dst), size);
  return size;
}

uint_t bstring::from(const byte *src)
{
  const uint_t size = this->size();
  this->mValue.clear();
  this->mValue.assign(reinterpret_cast<const char*>(src), size);
  return size;
}
#endif
