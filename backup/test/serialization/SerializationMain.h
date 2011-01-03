#ifndef RSSD_SERIALIZATION_MAIN_H
#define RSSD_SERIALIZATION_MAIN_H

#include "Core"
#include "TestBinarySerializer.h"

namespace RSSD {
namespace Serialization {

int SerializationMain(int argc, char **argv)
{
  TestBinarySerializer test;
  bint_t *var = static_cast<bint_t*>(test["mOne"]);
  var->set(5);
  std::cout << var->get() << std::endl;
  return 0;
}

} /// namespace Serialization
} /// namespace RSSD

#endif /// RSSD_SERIALIZATION_MAIN_H
