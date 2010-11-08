#include "Keyboard.h"

using namespace RSSD;
using namespace RSSD::Input;

Keyboard::State::State() :
  Device::State(Device::KEYBOARD),
  mKeyboardCode(0)
{

}

Keyboard::State::~State()
{

}

Keyboard::Event::Event(const uint32_t event) :
  Device::Event(Device::KEYBOARD, event)
{

}

Keyboard::Event::~Event()
{

}

Keyboard::Keyboard() :
  Device(new Keyboard::State())
{

}

Keyboard::~Keyboard()
{

}

bool Keyboard::update(const float64_t elapsed)
{
  return true;
}
