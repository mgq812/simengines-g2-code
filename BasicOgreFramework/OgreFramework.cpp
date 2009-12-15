//|||||||||||||||||||||||||||||||||||||||||||||||

#include "OgreFramework.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

using namespace Ogre;

//|||||||||||||||||||||||||||||||||||||||||||||||

template<> OgreFramework* Ogre::Singleton<OgreFramework>::ms_Singleton = 0;

//Set some default values
OgreFramework::OgreFramework()
{
	m_MoveSpeed			= 0.1;
	m_RotateSpeed		= 0.3;

	m_bShutDownOgre		= false;
	m_iNumScreenShots	= 0;

	m_pRoot				= 0;
	m_pSceneMgr			= 0;
	m_pRenderWnd		= 0;
	m_pCamera			= 0;
	m_pViewport			= 0;
	m_pLog				= 0;
	m_pTimer			= 0;

	m_pInputMgr			= 0;
	m_pKeyboard			= 0;
	m_pMouse			= 0;

	m_pDebugOverlay		= 0;
	m_pInfoOverlay		= 0;
}

//Initialize everyhing
void OgreFramework::initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener, OIS::MouseListener *pMouseListener)
{
	//To be able to write to the log
	Ogre::LogManager* logMgr = new Ogre::LogManager();
	
	m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
	m_pLog->setDebugOutputEnabled(true);
	
	//The root
	m_pRoot = new Ogre::Root();

	m_pRoot->showConfigDialog();
	m_pRenderWnd = m_pRoot->initialise(true, wndTitle);

	//The scene manager with some light
	m_pSceneMgr = m_pRoot->createSceneManager(ST_GENERIC, "SceneManager");
	m_pSceneMgr->setAmbientLight(Ogre::ColourValue(0.7, 0.7, 0.7));
	
	//Set up the camera
	m_pCamera = m_pSceneMgr->createCamera("Camera");
	m_pCamera->setPosition(Vector3(0, 60, 60));
	m_pCamera->lookAt(Vector3(0,0,0));
	m_pCamera->setNearClipDistance(1);

	//Viewport and backgroundcolor
	m_pViewport = m_pRenderWnd->addViewport(m_pCamera);
	m_pViewport->setBackgroundColour(ColourValue(0.8, 0.7, 0.6, 1.0));

	m_pCamera->setAspectRatio(Real(m_pViewport->getActualWidth()) / Real(m_pViewport->getActualHeight()));
	
	m_pViewport->setCamera(m_pCamera);

	//Create input devices
	unsigned long hWnd = 0;
    OIS::ParamList paramList;
    m_pRenderWnd->getCustomAttribute("WINDOW", &hWnd);

	paramList.insert(OIS::ParamList::value_type("WINDOW", Ogre::StringConverter::toString(hWnd)));

	m_pInputMgr = OIS::InputManager::createInputSystem(paramList);

    m_pKeyboard = static_cast<OIS::Keyboard*>(m_pInputMgr->createInputObject(OIS::OISKeyboard, true));
	m_pMouse = static_cast<OIS::Mouse*>(m_pInputMgr->createInputObject(OIS::OISMouse, true));
    
	m_pMouse->getMouseState().height = m_pRenderWnd->getHeight();
	m_pMouse->getMouseState().width	 = m_pRenderWnd->getWidth();

	if(pKeyListener == 0)
		m_pKeyboard->setEventCallback(this);
	else
		m_pKeyboard->setEventCallback(pKeyListener);

	if(pMouseListener == 0)
		m_pMouse->setEventCallback(this);
	else
		m_pMouse->setEventCallback(pMouseListener);

	//Load resources
	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile cf;
    cf.load("resources.cfg");

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
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	//Create the timer
	m_pTimer = new Ogre::Timer();
	m_pTimer->reset();
	
	//Create the debug overlay
	m_pDebugOverlay = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
	m_pDebugOverlay->show();

	//Activate the renderer
	m_pRenderWnd->setActive(true);
}

//Clean up
OgreFramework::~OgreFramework()
{
	if(m_pInputMgr)
	{
		delete m_pKeyboard;
		delete m_pMouse;
		//OIS::InputManager::destroyInputSystem(m_pInputMgr);
	}

	delete m_pRoot;
}

//Key pressed handeler
bool OgreFramework::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	m_pLog->logMessage("OgreFramework::keyPressed");
	
	//Esc to end the program
	if(m_pKeyboard->isKeyDown(OIS::KC_ESCAPE))
	{
			m_bShutDownOgre = true;
			return true;
	}

	//Printscreen to take printscreens
	if(m_pKeyboard->isKeyDown(OIS::KC_SYSRQ))
	{
		std::ostringstream ss;
		ss << "screenshot_" << ++m_iNumScreenShots << ".png";
		m_pRenderWnd->writeContentsToFile(ss.str());
		return true;
	}

	//M to change polygon mode
	if(m_pKeyboard->isKeyDown(OIS::KC_M))
	{
		static int mode = 0;
		
		if(mode == 2)
		{
			m_pCamera->setPolygonMode(PM_SOLID);
			mode = 0;
		}
		else if(mode == 0)
		{
			 m_pCamera->setPolygonMode(PM_WIREFRAME);
			 mode = 1;
		}
		else if(mode == 1)
		{
			m_pCamera->setPolygonMode(PM_POINTS);
			mode = 2;
		}
	}

	//O to hide the debug overlay
	if(m_pKeyboard->isKeyDown(OIS::KC_O))
	{
		if(m_pDebugOverlay)
		{
			if(!m_pDebugOverlay->isVisible())
				m_pDebugOverlay->show();
			else
				m_pDebugOverlay->hide();
		}
	}

	return true;
}

//Key released handeler
bool OgreFramework::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	return true;
}

//Mouse moved handeler
bool OgreFramework::mouseMoved(const OIS::MouseEvent &evt)
{
	m_pCamera->yaw(Degree(m_pMouse->getMouseState().X.rel * -0.1));
	m_pCamera->pitch(Degree(m_pMouse->getMouseState().Y.rel * -0.1));

	return true;
}

//Mouse pressed handeler
bool OgreFramework::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

//Mouse released handeler
bool OgreFramework::mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
	return true;
}

//OgreFramework update method
void OgreFramework::updateOgre(double timeSinceLastFrame)
{
	m_MoveScale = m_MoveSpeed   * timeSinceLastFrame;
	m_RotScale  = m_RotateSpeed * timeSinceLastFrame;
		
	m_TranslateVector = Vector3::ZERO;

	getInput();
	moveCamera();
	
	updateStats();
}

//Update the status window
void OgreFramework::updateStats() 
{ 
	static String currFps = "Current FPS: "; 
    static String avgFps = "Average FPS: "; 
    static String bestFps = "Best FPS: "; 
    static String worstFps = "Worst FPS: "; 
    static String tris = "Triangle Count: "; 
    static String batches = "Batch Count: "; 
 
    OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps"); 
    OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps"); 
    OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps"); 
    OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps"); 

	const RenderTarget::FrameStats& stats = m_pRenderWnd->getStatistics(); 
    guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS)); 
    guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS)); 
    guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS) 
            +" "+StringConverter::toString(stats.bestFrameTime)+" ms"); 
    guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS) 
            +" "+StringConverter::toString(stats.worstFrameTime)+" ms"); 

    OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris"); 
    guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount)); 

	OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches"); 
    guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount)); 

	OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText"); 
	guiDbg->setCaption("");
} 

//Change camera position, if shift is hold move it tenfold
void OgreFramework::moveCamera()
{
	if(m_pKeyboard->isKeyDown(OIS::KC_LSHIFT)) m_pCamera->moveRelative(m_TranslateVector);
	m_pCamera->moveRelative(m_TranslateVector / 10);
}

//Check keyboard input and translate accordingly
void OgreFramework::getInput()
{
	if(m_pKeyboard->isKeyDown(OIS::KC_LEFT))
	{
		m_pCamera->yaw(m_RotScale);
	}

	if(m_pKeyboard->isKeyDown(OIS::KC_RIGHT))
	{
		m_pCamera->yaw(-m_RotScale);
	}

	if(m_pKeyboard->isKeyDown(OIS::KC_UP))
	{
		m_pCamera->pitch(m_RotScale);
	}

	if(m_pKeyboard->isKeyDown(OIS::KC_DOWN))
	{
		m_pCamera->pitch(-m_RotScale);
	}
}

//|||||||||||||||||||||||||||||||||||||||||||||||