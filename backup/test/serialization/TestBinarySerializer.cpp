#include "TestBinarySerializer.h"

using namespace boost::assign;
using namespace RSSD;

TestBinarySerializer::DataMemberMap TestBinarySerializer::DATA_MEMBERS =
  map_list_of("mOne", new bint_t);

TestBinarySerializer::TestBinarySerializer() : BinarySerializer(TestBinarySerializer::DATA_MEMBERS)
{

}

TestBinarySerializer::~TestBinarySerializer()
{

}
