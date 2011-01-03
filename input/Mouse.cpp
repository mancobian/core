#include "Mouse.h"
#include <cstring>

using namespace std;
using namespace RSSD;
using namespace RSSD::Core;
using namespace RSSD::Core::Input;

DECLARE_SINGLETON(RSSD::Core::Input::Mouse::Factory);

Mouse::State::State() :
  Device::State(Mouse::Factory::getType())
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

Mouse::Event::Event(const uint_t eventType) :
  Device::Event(eventType)
{

}

Mouse::Event::~Event()
{

}

Mouse::Mouse() :
  BaseDevice<Mouse>(new Mouse::State())
{

}

Mouse::~Mouse()
{

}
