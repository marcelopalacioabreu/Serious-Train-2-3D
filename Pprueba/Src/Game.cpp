#include "Game.h"

Game::Game() :
	gameManager(this), inputManager(this),
	dataManager(this), graphicsManager(this),
	physicsManager(this), root(0),
	mResourcesCfg(Ogre::StringUtil::BLANK), 
	mPluginsCfg(Ogre::StringUtil::BLANK)
{
}

Game::~Game()
{

}

bool Game::start()
{
	root = new Ogre::Root(mPluginsCfg);

	//Rutas de recursos y plugins
	mResourcesCfg = "Ogre/resources.cfg";
	mPluginsCfg = "Ogre/plugins.cfg";
	

	cf.load(mPluginsCfg);
	//Setting UP Resources 

	//Parsing the config file into the system.
	cf.load(mResourcesCfg);


	//name: Path to resources in disk,
	//loctype: defines what kind of location the element is (e.g. Filesystem, zip..)
	Ogre::String name, locType;

	//We now iterate through rach section in the resources.cfg.
	//Sections are signaled as [NAME]
	Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
	while (secIt.hasMoreElements())
	{
		Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator it;

		//Now we are iterating INSIDE the section [secIt]
		for (it = settings->begin(); it != settings->end(); ++it)
		{
			locType = it->first;
			name = it->second;

			//We now know the type of the element and its path.
			//We add it as a location to the Resource Group Manager
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
			std::cout << "inserting:\n";
			std::cout << name << locType << std::endl;
		}
	}
	//------------------------------------------------------------------------------------------------------
	// Render System Config

	//If there is no previous Ogre.cfg, this displays the config dialog
	if (!(root->restoreConfig() || root->showConfigDialog()))
		return false;

	//------------------------------------------------------------------------------------------------------
	//Render Window Creation
	mWindow = root->initialise(true, "Griffindor");

	//------------------------------------------------------------------------------------------------------
	//Resources Init

	//We are only going to use 5 mimpams at a time. Mipmaps are efficent ways to save a texture.
	//Taking only 1/3 more of space, we can have several sizes of the texture to choose from.
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	//Now we init every resource previously added
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	//------------------------------------------------------------------------------------------------------
	//SceneManager Set Up

	//we generate the default sceneManager. (more SceneManagers in Ogre::ST_....)
	scnMgr = root->createSceneManager(Ogre::ST_GENERIC);

	//------------------------------------------------------------------------------------------------------
	//Camera Creation

	//Self-explanatory methods
	cam = scnMgr->createCamera("MainCam");
	cam->setPosition(0, 0, 80);
	cam->lookAt(0, 0, -300);
	cam->setNearClipDistance(5);


	//------------------------------------------------------------------------------------------------------
	//ViewPort Addition
	vp = mWindow->addViewport(cam);
	vp->setBackgroundColour(Ogre::ColourValue(150, 150, 150));

	cam->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));

	//------------------------------------------------------------------------------------------------------
	//Scene SetUp
	/*try {

		Ogre::Entity * robot = scnMgr->createEntity("ogrehead.mesh");
		Ogre::SceneNode * robotNode = scnMgr->getRootSceneNode()->createChildSceneNode();
		robotNode->attachObject(robot);
	}
	catch (Ogre::FileNotFoundException e) {
		std::string a = e.getFullDescription();
		std::cout << a;
	}*/
	scnMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

	light = scnMgr->createLight("MainLight");
	light->setPosition(20, 80, 50);

	return true;
}

bool Game::stop()
{
	return false;
}

void Game::run()
{
	while (true)
	{
		Ogre::WindowEventUtilities::messagePump();
		if (mWindow->isClosed())return;
		if (!root->renderOneFrame())return;
	}

}
GameManager Game::getGameManager() const
{
	return gameManager;
}

InputManager Game::getInputManager() const
{
	return inputManager;
}

GraphicsManager Game::getGraphicsManager() const
{
	return graphicsManager;
}

DataManager Game::getDataManager() const
{
	return dataManager;
}

PhysicsManager Game::getPhysicsManager() const
{
	return physicsManager;
}

