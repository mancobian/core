#include "OgreCameraController.h"
#include <log/Log.h>
#include <OGRE/Ogre.h>

using namespace Nous;
using namespace Nous::App;

OgreCameraController::OgreCameraController(Ogre::SceneManager *scene_manager, Ogre::Camera *camera) :
	CameraController(),
	_camera(camera),
	_camera_node(NULL),
	_scene_manager(scene_manager)
{
	// Zero out translation and rotation
	std::memset(&this->_translation, 0, sizeof(this->_translation));
	std::memset(&this->_rotation, 0, sizeof(this->_translation));

	// Create camera and node
	this->_camera_node = this->_scene_manager->getRootSceneNode()->createChildSceneNode("cam_node");
	this->_camera_node->attachObject(this->_camera);

	// Configure main camera
	this->_camera_node->setPosition(Ogre::Vector3(0.0f, 0.0f, 120.0f));
	// this->_camera_node->lookAt(Ogre::Vector3::ZERO, Ogre::Node::TS_WORLD);
}

OgreCameraController::~OgreCameraController()
{
	if (this->_camera_node)
		this->_camera_node->removeAndDestroyAllChildren();
	if (this->_scene_manager)
		this->_scene_manager->destroySceneNode(this->_camera_node->getName());
}

bool OgreCameraController::update(const float32 elapsed)
{
#if 0
	// Reset frame variables
	this->_move_scale = this->_move_speed * elapsed;
	this->_rotate_scale = this->_rotate_speed * elapsed;
	this->_translation_vector = Ogre::Vector3::ZERO;
#endif

	Ogre::Vector3 translation(
		this->_translation.x,
		this->_translation.y,
		this->_translation.z);

	if (this->_is_turbo_enabled)
		translation *= CameraController::MOVE_MULTIPLIER;

	Ogre::Quaternion rotx(Ogre::Radian(Ogre::Degree(this->_rotation.x)), Ogre::Vector3::UNIT_X);
	Ogre::Quaternion roty(Ogre::Radian(Ogre::Degree(this->_rotation.y)), Ogre::Vector3::UNIT_Y);
	Ogre::Quaternion rotz(Ogre::Radian(Ogre::Degree(this->_rotation.z)), Ogre::Vector3::UNIT_Z);
	Ogre::Quaternion ori = this->_camera_node->getOrientation();
	ori = ori * (rotx * roty * rotz);

	this->_camera_node->yaw(Ogre::Radian(Ogre::Degree(this->_rotation.y)), Ogre::Node::TS_WORLD);
	this->_camera_node->pitch(Ogre::Radian(Ogre::Degree(this->_rotation.x)), Ogre::Node::TS_LOCAL);
	this->_camera_node->translate(
		this->_camera_node->getOrientation() * translation,
		Ogre::Node::TS_WORLD);

	// Reset node transformation accumulators
	std::memset(&this->_translation, 0, sizeof(this->_translation));
	std::memset(&this->_rotation, 0, sizeof(this->_translation));
	return true;
}
