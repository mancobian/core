#include "input/ois/OisKeyboard.h"
#include "input/ois/OisInputManager.h"

using namespace RSSD;
using namespace RSSD::Core;
using namespace RSSD::Core::Input;

DECLARE_SINGLETON(RSSD::Core::Input::OisKeyboard::Factory);

OisKeyboard::OisKeyboard(OIS::Keyboard *keyboard) :
  Keyboard(),
  OisDevice(),
  mKeyboard(keyboard)
{

}

OisKeyboard::~OisKeyboard()
{

}
