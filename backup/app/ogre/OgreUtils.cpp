#include "OgreUtils.h"
#include <log/Log.h>

using namespace Nous;
using namespace Nous::App;

template<> OgreUtils* Ogre::Singleton<OgreUtils>::ms_Singleton = 0;

OgreUtils::OgreUtils()
{

}

OgreUtils::~OgreUtils()
{

}

void OgreUtils::printResourceGroup(const std::string &name) const
{
	// Local vars
	std::stringstream msg;
	Ogre::ResourceGroupManager *mgr = Ogre::ResourceGroupManager::getSingletonPtr();

	// Get resource list for the requested resource group
	Ogre::StringVectorPtr resources = mgr->listResourceNames(name, false);
	msg << "[RESOURCE] -----\n";
	msg << "[RESOURCE] RESOURCE GROUP INFO" << "\n";
	msg << "[RESOURCE] -----\n";
	msg << "[RESOURCE] NAME: " << name << "\n";
	msg << "[RESOURCE] COUNT: " << resources->size() << "\n";
#if (OGRE_VERSION_MAJOR >= 1) && (OGRE_VERSION_MINOR >= 6)
	msg << "[RESOURCE] IS INIT: " << mgr->isResourceGroupInitialised(name) << "\n";
	msg << "[RESOURCE] IS LOADED: " << mgr->isResourceGroupLoaded(name) << "\n";
#endif
	msg << "[RESOURCE] -----\n";
	Ogre::StringVector::iterator iter = resources->begin(),
		end = resources->end();
	while (iter != end)
	{
		Ogre::String &resource = *iter; ++iter;
		msg << "[RESOURCE] => " << resource << "\n";
	}
	msg << "[RESOURCE] -----";
	LOG (msg.str());
}
