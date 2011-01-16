///
/// @file OISInputManager.h
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

#ifndef NOUS_INPUT_OISINPUTMANAGER_H
#define NOUS_INPUT_OISINPUTMANAGER_H

#include <OIS/OIS.h>
#include <core/Core.h>
#include <pattern/Singleton.h>
#include <pattern/Manager.h>
#include <input/InputManager.h>

namespace Ois {
class InputManager;
class Keyboard;
class Mouse;
TYPEDEF_PTR_CONTAINERS(Keyboard);
TYPEDEF_PTR_CONTAINERS(Mouse);
typedef std::list<Keyboard*> KeyboardManager_l;
typedef std::list<Mouse*> MouseManager_l;
} // namespace Ois

namespace Nous {
namespace Input {

///
/// @class OISInputManager
///

class OISInputManager :
	public Pattern::Singleton<OISInputManager>,
	public OIS::KeyListener,
	public OIS::MouseListener,
	public InputManager
{
public:
	typedef InputManager Base;

public:
	OISInputManager(const uint32_t hwnd,
		const uint32_t width,
		const uint32_t height);
	virtual ~OISInputManager();

public:
	virtual void create();
	virtual void create(const uint32_t hwnd,
		const uint32_t width,
		const uint32_t height);
	virtual void destroy();
	virtual bool update(const float32_t elapsed);

protected:
	virtual bool keyPressed(const OIS::KeyEvent &key_event);
	virtual bool keyReleased(const OIS::KeyEvent &key_event);
	virtual bool mouseMoved(const OIS::MouseEvent &mouse_event);
	virtual bool mousePressed(const OIS::MouseEvent &mouse_event, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &mouse_event, OIS::MouseButtonID id);

protected:
	OIS::InputManager* _input_manager;
	OIS::KeyboardManager_l _keyboards;
	OIS::MouseManager_l _mice;
}; // class OISInputManager

} // namespace Input
} // namespace Nous

#endif // NOUS_INPUT_OISINPUTMANAGER_H
