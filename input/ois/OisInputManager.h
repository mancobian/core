///
/// @file OisInputManager.h
/// @author Mancobian Poemandres
/// @license BSD License
///
/// Copyright (c) MMX by The Secret Design Collective
/// All rights reserved
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
///    * Redistributions of source code must retain the above copyright notice,
/// 		this list of conditions and the following disclaimer.
///    * Redistributions in binary form must reproduce the above copyright notice,
/// 		this list of conditions and the following disclaimer in the documentation
/// 		and/or other materials provided with the distribution.
///    * Neither the name of The Secret Design Collective nor the names of its
/// 		contributors may be used to endorse or promote products derived from
/// 		this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
/// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
/// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
/// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
/// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
/// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
/// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
/// USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///

#ifndef RSSD_CORE_INPUT_OISINPUTMANAGER_H
#define RSSD_CORE_INPUT_OISINPUTMANAGER_H

#include <OIS/OIS.h>
#include "System"
#include "Pattern"
#include "input/Device.h"
#include "input/ois/OisTraits.h"

namespace RSSD {
namespace Core {
namespace Input {
namespace Impl {

class OisInputManager : public Pattern::Manager<Device*>
  // public ::OIS::JoyStickListener
{
public:
  typedef OisTraits Traits;
  typedef Pattern::Manager<Device*> DeviceManager;

  OisInputManager(Traits::WindowHandleType windowHandle = 0);
  virtual ~OisInputManager();
  static OIS::Type toOisType(const uint32_t type);
  bool isDeviceAvailable(const OIS::Type type) const;
  Device* createDevice(const uint32_t type);
  Device* createDevice(const uint32_t type, const bool isBuffered);
  bool destroyDevice(Device *device);
  bool update(const float_t elapsed);
  bool create(const Traits::WindowHandleType windowHandle);
  bool destroy();

protected:
  OIS::InputManager *mInputSystem;

  /*
  virtual bool povMoved( const OIS::JoyStickEvent &e, int pov );
  virtual bool axisMoved( const OIS::JoyStickEvent &e, int axis );
  virtual bool sliderMoved( const OIS::JoyStickEvent &e, int sliderID );
  virtual bool buttonPressed( const OIS::JoyStickEvent &e, int button );
  virtual bool buttonReleased( const OIS::JoyStickEvent &e, int button );
  */
}; /// class OisInputManager

} /// namespace Impl
} /// namespace Input
} /// namespace Core
} /// namespace RSSD

#endif // RSSD_INPUT_OISINPUTMANAGER_H
