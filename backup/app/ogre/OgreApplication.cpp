#include <log/Log.h>
#include <input/Input.h>
#include <interface/Interface.h>
#include <timer/Timer.h>
#include <app/configuration/configuration.pb.h>
#include "OgreApplication.h"
#include "Defines.h"
#include "OgreUtils.h"
#include "ogre/OgreCameraController.h"

using namespace std;
using namespace Ogre;
using namespace Nous;
using namespace Nous::App;

const char* OgreApplication::NAME = "Nous";
const char* OgreApplication::RESOURCES_FILE = "../resources/config/resources.cfg";
const char* OgreApplication::PLUGINS_FILE = "../resources/config/plugins.cfg";
const char* OgreApplication::NLOG_FILE = "../resources/config/ogre.log";
const char* OgreApplication::CONFIG_FILE = "../resources/config/ogre.cfg";
const char* OgreApplication::SCENE_RESOURCE_GROUP = "Scene";
const char* OgreApplication::NOUS_CONFIGURATION_FILE = "../resources/config/nous.cfg";

OgreApplication::OgreApplication() :
	BaseApplication(),
	_num_frames(0),
	_num_lights_remaining(0),
	_elapsed_time(0.0f),
	_name(OgreApplication::NAME)
{	
	this->create();
}

OgreApplication::~OgreApplication()
{	
	// Cleanup the application environment
	this->destroy();

#ifdef __linux
	// HACK: Re-enable key-repeat
	system("xset r on");
#endif
}

void OgreApplication::start()
{
	// TODO: Integrate scene loading better
	// Load the scene
	if (!this->_scene_name.empty())
		this->loadScene(this->_scene_name);

	// Main loop, go!
	this->run();
}

bool OgreApplication::quit(const Input::DeviceEvent &event)
{
	this->_is_running = false;
	return true;
}

bool OgreApplication::stop()
{
	// TODO: Integrate scene loading better
	// Unload the scene
	this->unloadScene();

	// Update running flag
	this->_is_running = false;
	return true;
}

void OgreApplication::create()
{
	// Init core
	this->createCore();

	// Init Ogre system
	this->createOgre();

	NLOG ("");
	NLOG ("--------------------", "APP");
	NLOG ("--- BEGIN CREATE ---", "APP");
	NLOG ("--------------------", "APP");
	NLOG ("");
	
	// Init display
	this->createDisplay();

	// Initialize input
	this->createInput();

	// Init global resource system
	// NOTE: This is order-dependent and must occur
	// after the input system has been created,
	// since the UI system requires a valid
	// input manager for initialization
	this->createResources();

	// Create event system
	this->createEventSystem();

#if 0
	// Print all loaded resources
	OgreUtils::getSingletonPtr()->printResourceGroup(
		Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
#endif

	NLOG ("");
	NLOG ("------------------", "APP");
	NLOG ("--- END CREATE ---", "APP");
	NLOG ("------------------", "APP");
	NLOG ("");
}

void OgreApplication::destroy()
{
	// Destroy event system
	this->destroyEventSystem();

	// Destroy resources
	this->destroyResources();

	// Destroy input
	this->destroyInput();

	// Destroy Ogre root object
	this->destroyOgre();

	NLOG ("Application shutdown successfully!");

	// Destroy core
	this->destroyCore();
}

void OgreApplication::createCore()
{
	new Nous::LogManager();
	new Nous::TimerManager();

	this->_timer = Nous::TimerManager::getPointer()->createTimer(Platform::POSIX);
}

void OgreApplication::destroyCore()
{
	delete Nous::LogManager::getPointer();
	delete Nous::TimerManager::getPointer();
}

void OgreApplication::createEventSystem()
{
	// Register event producers
	assert (this->_input_manager != NULL);
	this->addEventProducer(this->_input_manager);
}

void OgreApplication::destroyEventSystem()
{
	// Remove all event producers
	this->clearEventProducers();
}

void OgreApplication::createOgre()
{
	// Create log manager
	new Ogre::LogManager();
	this->_log = Ogre::LogManager::getSingleton().createLog(
		OgreApplication::NLOG_FILE, true, true, false);
	this->_log->setDebugOutputEnabled(true);
	
	// Create OgreUtils
	new OgreUtils();

	// Create Ogre root object
	this->_root = new Ogre::Root(
		OgreApplication::PLUGINS_FILE,
		OgreApplication::CONFIG_FILE);

	// Create scene manager
	this->_scene_manager = this->_root->createSceneManager(
		Ogre::ST_GENERIC,
		"MainScene");

	// Setup app configuration and initialize app
	if (!this->_root->restoreConfig())
	{
		if (!this->_root->showConfigDialog())
		{
			assert (false && "Unable to load or create Ogre configuration!");
			exit(1);
		}
	}
	
#if 0
	// Configure geometry manager
	new GeometryManager();
	GeometryManager::getSingletonPtr()->SceneManager(this->_scene_manager);
#endif
}

void OgreApplication::destroyOgre()
{
	// Destroy app resources
	this->destroyResources();

	if (Ogre::Root::getSingletonPtr())
		delete Ogre::Root::getSingletonPtr();

	if (Ogre::LogManager::getSingletonPtr())
		delete Ogre::LogManager::getSingletonPtr();

	if (OgreUtils::getSingletonPtr())
		delete OgreUtils::getSingletonPtr();
}

void OgreApplication::createDisplay()
{
	// Create the main display window
	this->_render_window = this->_root->initialise(true, this->_name.c_str());
	assert (this->_render_window->isAutoUpdated());

	// Create and configure camera
	this->_camera = this->_scene_manager->createCamera("camera");
	this->_camera->setNearClipDistance(1.0f);
	this->_camera->setFarClipDistance(12000.0f);
	this->_camera->setFOVy(Ogre::Degree(60.0f));

	// Create camera controller
	this->_main_camera_controller = new OgreCameraController(this->_scene_manager, this->_camera);

	// Create main viewport
	this->_viewport = this->_render_window->addViewport(this->_camera);
	this->_viewport->setBackgroundColour(Ogre::ColourValue(0.85, 0.85, 0.0, 0.0));

	// Configure camera aspect ratio
	this->_camera->setAspectRatio(Real(this->_viewport->getActualWidth()) / Real(this->_viewport->getActualHeight()));
	this->_viewport->setCamera(this->_camera);

	// Active main render window
	this->_render_window->setActive(true);
}

void OgreApplication::createUI()
{
	// Create UI manager
	new Interface::Manager();

	// Create a test page
	Interface::Region *region = Interface::Manager::getPointer()->createPage(640, 480);
	assert (page != NULL);
	// page->setURL("../resources/media/html/index.html");
	page->setURL("http://www.google.com");
}

void OgreApplication::destroyUI()
{
	// Destroy UI system
	if (Interface::Manager::getPointer())
		delete Interface::Manager::getPointer();
}

void OgreApplication::createInput()
{
	using namespace Nous::Input;

	// Create input manager
	unsigned long hwnd = 0;
	this->_render_window->getCustomAttribute("WINDOW", &hwnd);
	new Input::OISInputManager(hwnd,
		this->_render_window->getWidth(),
		this->_render_window->getHeight());

#if 0
	Input::OISInputManager::getPointer()->addKeyboardListener(
		this->_main_camera_controller, &CameraController::moveForward,
		Keyboard::KEY_DOWN, OIS::KC_W);

	Input::OISInputManager::getPointer()->addKeyboardListener(
		this->_main_camera_controller, &CameraController::moveBackward,
		Keyboard::KEY_DOWN, OIS::KC_S);

	Input::OISInputManager::getPointer()->addKeyboardListener(
		this->_main_camera_controller, &CameraController::moveLeft,
		Keyboard::KEY_DOWN, OIS::KC_A);

	Input::OISInputManager::getPointer()->addKeyboardListener(
		this->_main_camera_controller, &CameraController::moveRight,
		Keyboard::KEY_DOWN, OIS::KC_D);

	Input::OISInputManager::getPointer()->addKeyboardListener(
		this->_main_camera_controller, &CameraController::moveUp,
		Keyboard::KEY_DOWN, OIS::KC_E);

	Input::OISInputManager::getPointer()->addKeyboardListener(
		this->_main_camera_controller, &CameraController::moveDown,
		Keyboard::KEY_DOWN, OIS::KC_C);

	Input::OISInputManager::getPointer()->addKeyboardListener(
		this->_main_camera_controller, &CameraController::rotateLeft,
		Keyboard::KEY_DOWN, OIS::KC_LEFT);

	Input::OISInputManager::getPointer()->addKeyboardListener(
		this->_main_camera_controller, &CameraController::rotateRight,
		Keyboard::KEY_DOWN, OIS::KC_RIGHT);

	Input::OISInputManager::getPointer()->addKeyboardListener(
		this->_main_camera_controller, &CameraController::rotateUp,
		Keyboard::KEY_DOWN, OIS::KC_UP);

	Input::OISInputManager::getPointer()->addKeyboardListener(
		this->_main_camera_controller, &CameraController::rotateDown,
		Keyboard::KEY_DOWN, OIS::KC_DOWN);

	Input::OISInputManager::getPointer()->addKeyboardListener(
		this, &OgreApplication::quit,
		Keyboard::KEY_DOWN, OIS::KC_ESCAPE);

	Input::OISInputManager::getPointer()->addKeyboardListener(
		this->_main_camera_controller, &CameraController::disableTurbo,
		Keyboard::KEY_RELEASE, OIS::KC_LSHIFT);

	Input::OISInputManager::getPointer()->addKeyboardListener(
		this->_main_camera_controller, &CameraController::enableTurbo,
		Keyboard::KEY_PRESS, OIS::KC_LSHIFT);

#if 0
	if (this->_keyboard->isKeyDown(OIS::KC_ESCAPE))
	{
		this->stop();
		return true;
	}
	if (this->_keyboard->isKeyDown(OIS::KC_M))
	{
		static int mode = 0;
		if (mode == 2)
		{
			this->_camera->setPolygonMode(PM_SOLID);
			mode = 0;
		}
		else if (mode == 0)
		{
			 this->_camera->setPolygonMode(PM_WIREFRAME);
			 mode = 1;
		}
		else if (mode == 1)
		{
			this->_camera->setPolygonMode(PM_POINTS);
			mode = 2;
		}
	}
	if (this->_keyboard->isKeyDown(OIS::KC_O))
	{
		if (this->_debug_overlay)
		{
			if (!this->_debug_overlay->isVisible())
				this->_debug_overlay->show();
			else
				this->_debug_overlay->hide();
		}
	}
	if (this->_keyboard->isKeyDown(OIS::KC_EQUALS))
	{
		static const std::string PREFIX = OgreApplication::NAME;
		static const std::string SUFFIX = ".png";
		Ogre::Root::getSingleton().getAutoCreatedWindow()->writeContentsToTimestampedFile(PREFIX, SUFFIX);
	}
#endif
#endif
}

void OgreApplication::destroyInput()
{
	// Destroy input manager
	if (Input::OISInputManager::getPointer())
		delete Input::OISInputManager::getPointer();
}

void OgreApplication::createResources()
{
	// Load configuration file
	Ogre::String secName,
		typeName,
		archName;
	Ogre::ConfigFile cf;
	cf.load(OgreApplication::RESOURCES_FILE);

	// Initialize scene resources from configuration file
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	while (seci.hasMoreElements())
	{
		secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for (i = settings->begin(); i != settings->end(); ++i)
		{
			typeName = i->first;
			archName = i->second;
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
		}
	}

	// Initalize global resources
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	// Configure default scene properties
	this->_scene_manager->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7));

	// Configure textures
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// Create overlays
	this->_debug_overlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
	this->_debug_overlay->show();

	// Create scene loader
	this->_ofusion_scene_loader = new OSMScene(
		this->_scene_manager,
		this->_render_window);

	// Initialize UI
	this->createUI();
}

void OgreApplication::destroyResources()
{
	// Destroy UI
	this->destroyUI();

	// Destroy oFusion scene loader
	delete this->_ofusion_scene_loader;
	this->_ofusion_scene_loader = NULL;
}

void OgreApplication::loadSceneResources()
{
	// Create scene resource group
	ResourceGroupManager::getSingletonPtr()->createResourceGroup(OgreApplication::SCENE_RESOURCE_GROUP);
}

void OgreApplication::unloadSceneResources()
{
	// Destroy scene resource group
	ResourceGroupManager::getSingletonPtr()->destroyResourceGroup(OgreApplication::SCENE_RESOURCE_GROUP);
}

void OgreApplication::loadScene(const std::string &file)
{
	// Load scene resources
	this->loadSceneResources();

	// Load the specified scene
	this->_ofusion_scene_loader->initialise(file.c_str());
	this->_ofusion_scene_loader->createScene();
}

void OgreApplication::unloadScene()
{
	// Unload scene resources
	this->unloadSceneResources();
}

void OgreApplication::updateStats(const double elapsed_time)
{
	static Ogre::String currFps = "Current FPS: "; 
	static Ogre::String avgFps = "Average FPS: "; 
	static Ogre::String bestFps = "Best FPS: "; 
	static Ogre::String worstFps = "Worst FPS: "; 
	static Ogre::String tris = "Triangle Count: "; 
	static Ogre::String batches = "Batch Count: "; 

	Ogre::OverlayElement* guiAvg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/AverageFps"); 
	Ogre::OverlayElement* guiCurr = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/CurrFps"); 
	//Ogre::OverlayElement* guiBest = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
	//Ogre::OverlayElement* guiWorst = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

	//size_t current_fps = (size_t)( 1000.0f / elapsed_time );
	//size_t average_fps = (size_t)( (this->_num_frames * 1000.0f) / this->_elapsed_time );
	const Ogre::RenderTarget* render_target = this->_render_window->getViewport(0)->getTarget();
	
	const Ogre::RenderTarget::FrameStats& stats = this->_render_window->getStatistics(); 
	guiAvg->setCaption(avgFps + Ogre::StringConverter::toString(stats.avgFPS)); 
	guiCurr->setCaption(currFps + Ogre::StringConverter::toString(stats.lastFPS)); 
	
	Ogre::OverlayElement* guiTris = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumTris"); 
	size_t triangle_count = render_target->getTriangleCount();
	guiTris->setCaption(tris + Ogre::StringConverter::toString(triangle_count)); 

	Ogre::OverlayElement* guiBatches = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumBatches"); 
	size_t batch_count = render_target->getBatchCount();
    guiBatches->setCaption(batches + Ogre::StringConverter::toString(batch_count)); 

#if 0
	Ogre::OverlayElement* guiDbg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/DebugText"); 
	std::stringstream ss;
	ss << "Current pos: x: " << this->_cam_node_yaw->_getDerivedPosition().x
		<< ", y: " << this->_cam_node_yaw->_getDerivedPosition().y
		<< ", z: " << this->_cam_node_yaw->_getDerivedPosition().z
		<< "\nCurrent orientation: " << Ogre::StringConverter::toString(this->_cam_node_yaw->_getDerivedOrientation());
	guiDbg->setCaption(ss.str());
#endif
	
#if 0
	NLOG (ss.str());
	NLOG (guiAvg->getCaption());
	NLOG (guiCurr->getCaption());
	NLOG (guiBest->getCaption());
	NLOG (guiWorst->getCaption());
	NLOG (guiTris->getCaption());
	NLOG (guiBatches->getCaption());
#endif
}

void OgreApplication::run()
{
	NLOG ("");
	NLOG ("-----------------------", "APP");
	NLOG ("--- BEGIN MAIN LOOP ---", "APP");
	NLOG ("-----------------------", "APP");
	NLOG ("");

	this->_is_running = true;

	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
	
	std::stringstream ss;
	float32_t elapsed = 0;
	float32_t last = 0;
	
	// Store frame start time
	this->_timer->reset();
	this->_timer->start();
	
	this->_render_window->resetStatistics();

	this->_render_window->setActive(true);

	while (this->_is_running)
	{
		if (this->_render_window->isClosed()) 
		{
			this->stop();
			break;
		}

//#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		// Flush system message queue
		Ogre::WindowEventUtilities::messagePump();
//#endif

		/// @todo Sleep here?
		if (!this->_render_window->isActive())
			continue;

		// Calculate elapsed frame time
		elapsed = this->_timer->getMilliseconds() - last;
		last = this->_timer->getMilliseconds();

		// Render one frame
		this->preRender(elapsed);
		this->render(elapsed);
		this->postRender(elapsed);
	}

	NLOG ("");
	NLOG ("---------------------", "APP");
	NLOG ("--- END MAIN LOOP ---", "APP");
	NLOG ("---------------------", "APP");
	NLOG ("");
}

bool OgreApplication::processEventStream()
{
	// Aggregate application event streams
	assert (Event::Manager::processEventStream());

	// Handle known events in event stream
	Event::Stream::Packet event;
	while (this->read(&event))
	{
		/// todo Implement this!
	}
	return true;
}

///
/// @note Event stream(s) should only be processed AFTER
/// event producers have been updated for the current frame.
///

void OgreApplication::preRender(const float32_t elapsed)
{
	// Handle per frame updates
	this->updateStats((double)elapsed);

	// Update input
	if (Input::OISInputManager::getPointer())
		Input::OISInputManager::getPointer()->update(elapsed);

	// Update camera
	if (this->_main_camera_controller)
		this->_main_camera_controller->update(elapsed);

	// Update UI
	if (Interface::PageManager::getPointer())
		Interface::PageManager::getPointer()->update(elapsed);

	// Process application event streams
	this->processEventStream();
}

void OgreApplication::render(const float32_t elapsed)
{
	this->_root->renderOneFrame();
}

void OgreApplication::postRender(const float32_t elapsed)
{

}
