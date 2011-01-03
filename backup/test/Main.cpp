#include "Core"
#include "test/filesystem/FilesystemMain.h"
#include "test/serialization/SerializationMain.h"

int main(int argc, char **argv)
{
  int result = RSSD::Serialization::SerializationMain(argc, argv);
  return result;
}
