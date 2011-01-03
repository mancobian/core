#include "Keyboard.h"

using namespace RSSD;
using namespace RSSD::Core;
using namespace RSSD::Core::Input;

DECLARE_SINGLETON(RSSD::Core::Input::Keyboard::Factory);

Keyboard::State::State() :
  Device::State(Keyboard::Factory::getType())
{
  std::memset(this->mKeys, '\0', sizeof(mKeys));
}

Keyboard::State::~State()
{

}

Keyboard::Event::Event(const uint32_t eventType) :
  Device::Event(Keyboard::Factory::getType(), eventType)
{

}

Keyboard::Event::~Event()
{

}

Keyboard::Keyboard() :
  BaseDevice<Keyboard>(new Keyboard::State())
{

}

Keyboard::~Keyboard()
{

}
