#include "Mouse.h"
#include <cstring>

using namespace std;
using namespace RSSD;
using namespace RSSD::Input;

Mouse::State::State() :
  Device::State(Device::MOUSE)
{
  /// Initialize mouse button states
  for (uint32_t i = 0; i < Button::COUNT; ++i)
  {
    this->mButtons.insert(std::make_pair(i, 0));
  }
}

Mouse::State::~State()
{

}

Mouse::Event::Event(const uint32_t event) :
  Device::Event(Device::MOUSE, event)
{

}

Mouse::Event::~Event()
{

}

Mouse::Mouse() :
  Device(new Mouse::State())
{

}

Mouse::~Mouse()
{

}

bool Mouse::update(const float64_t elapsed)
{
  return true;
}
