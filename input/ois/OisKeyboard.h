///
/// @file OisKeyboard.h
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

#ifndef RSSD_CORE_INPUT_OISKEYBOARD_H
#define RSSD_CORE_INPUT_OISKEYBOARD_H

#include <OIS/OIS.h>
#include "input/Keyboard.h"
#include "input/ois/OisDevice.h"

namespace RSSD {
namespace Core {
namespace Input {

class OisKeyboard :
  public Keyboard,
  public OisDevice
{
public:
  typedef Device::Factory::Impl<OisKeyboard> Factory;
  typedef SharedPointer<OisKeyboard> Pointer;

  OisKeyboard(OIS::Keyboard *keyboard);
  virtual ~OisKeyboard();
  OIS::Object *getOisObject() { return this->mKeyboard; }
  virtual bool update(const float_t elapsed) { return true; }

protected:
  OIS::Keyboard *mKeyboard;
}; /// class OisKeyboard

} /// namespace Input
} /// namespace Core
} /// namespace RSSD

#endif // RSSD_CORE_INPUT_OISKEYBOARD_H
