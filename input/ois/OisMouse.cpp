#include "input/ois/OisMouse.h"
#include "input/ois/OisInputManager.h"

using namespace RSSD;
using namespace RSSD::Core;
using namespace RSSD::Core::Input;

DECLARE_SINGLETON(RSSD::Core::Input::OisMouse::Factory);

OisMouse::OisMouse(OIS::Mouse *mouse) :
  Mouse(),
  OisDevice(),
  mMouse(mouse)
{

}

OisMouse::~OisMouse()
{

}
