///
/// @file OgreApplication.h
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

#ifndef NOUS_APP_OGREAPPLICATION_H
#define NOUS_APP_OGREAPPLICATION_H

#include <time.h>
#include <iostream>
#include <map>
#include <OGRE/Ogre.h>
#include <ofusion/OgreOSMScene.h>
#include <app/Application.h>

namespace Nous {
class Timer;
} // namespace Nous

namespace Nous {
namespace App {
class CameraController;
} // namespace App
} // namespace Nous

namespace Nous {
namespace Input {
class InputManager;
class DeviceEvent;
} // namespace Input
} // namespace Nous

namespace Nous {
namespace App {

class OgreApplication : public BaseApplication
{
public:
	OgreApplication();
	virtual ~OgreApplication();

public:
	virtual const char* getName() const { return OgreApplication::NAME; }
	virtual const char* getVersion() const { return "" /* VERSION */; }

public:
	inline Ogre::SceneManager* getSceneManager() { return this->_scene_manager; }
	inline Ogre::Camera* getCamera() { return this->_camera; }
	inline void setSceneName(const std::string &value) { this->_scene_name = value; }
	inline std::string getSceneName() const { return this->_scene_name; }

public:
	virtual void start();
	virtual bool stop();
	virtual void loadScene(const std::string &file);
	virtual void unloadScene();
	bool quit(const Input::DeviceEvent &event);

protected:
	virtual bool processEventStream();
	void create();
	void destroy();
	void run();
	void createCore();
	void destroyCore();
	void createEventSystem();
	void destroyEventSystem();
	void createOgre();
	void destroyOgre();
	void createResources();
	void destroyResources();
	void createDisplay();
	void createUI();
	void destroyUI();
	void createInput();
	void destroyInput();
	void loadSceneResources();
	void unloadSceneResources();
	void updateStats(const double elapsed_time);
	void preRender(const float32_t elapsed);
	void render(const float32_t elapsed);
	void postRender(const float32_t elapsed);

protected:
	static const char* NAME;
	static const char* SCENE_RESOURCE_GROUP;
	static const char* RESOURCES_FILE;
	static const char* PLUGINS_FILE;
	static const char* LOG_FILE;
	static const char* CONFIG_FILE;
	static const char* NOUS_CONFIGURATION_FILE;

protected:
	size_t _num_frames,
		_num_lights_remaining;
	float _elapsed_time;
	std::string _name,
		_scene_name,
		_dicom_file;
	Nous::App::CameraController *_main_camera_controller;
	Nous::Timer *_timer;
	OSMScene *_ofusion_scene_loader;
	Ogre::Root* _root;
	Ogre::SceneManager* _scene_manager;
	Ogre::RenderWindow* _render_window;
	Ogre::Camera* _camera;
	Ogre::Viewport* _viewport;
	Ogre::Log* _log;
	Ogre::CompositorInstance *_deferred_lighting_compositor;
	Ogre::Overlay* _debug_overlay;
	Ogre::Overlay* _info_overlay;
	Ogre::LightList _lights;
	Ogre::Texture *_light_colour_sampler;
	Ogre::SceneNode *_scene_root_node,
		*_light_root_node,
		*_cam_node_pitch,
		*_cam_node_yaw,
		*_cam_light_node;
	Nous::Input::InputManager *_input_manager;
}; // class OgreApplication

} // namespace App
} // namespace Nous

#endif // NOUS_APP_OGREAPPLICATION_H
