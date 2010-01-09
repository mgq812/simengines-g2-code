#ifndef OGRE_FRAMEWORK_HPP
#define OGRE_FRAMEWORK_HPP


#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

using namespace Ogre;

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

//Extensions
#include <Soundplayer.h>
#include <Astar.h>
#include <AstarNode.h>
#include <CartoonSystem.h>
#include <ProjectileCannon.h>
#include <NxOgre.h>
#include <NxOgreOGRE3D.h>


//|||||||||||||||||||||||||||||||||||||||||||||||

class OgreFramework : public Ogre::Singleton<OgreFramework>, OIS::KeyListener, OIS::MouseListener
{
public:
	/** Default constructor. Sets some default values.
        */
	OgreFramework();
	/** Default destructor.
        */
	~OgreFramework();

	/** Initialize everything about Ogre.
        @param wndTitle - String for the window name
		@param pKeyListener - KeyListerner
		@param pMouseListener - MouseListerner
        */
	void initOgre(Ogre::String wndTitle, OIS::KeyListener *pKeyListener = 0, OIS::MouseListener *pMouseListener = 0);
	/** Update everything about the Ogre.
        @param timeSinceLastFrame - The time since the last frame
        */
	void updateOgre(double timeSinceLastFrame);
	/** Update the Ogre statistics.
        */
	void updateStats();
	/** Change camera position, if shift is hold move it tenfold.
        */
	void moveCamera();
	/** Check keyboard input and translate accordingly.
        */
	void getInput();
	
	/** Check if Ogre should be shut down
		@return True if application should be shut down
        */
	bool isOgreToBeShutDown()const{return m_bShutDownOgre;}  
/** Checks if a key is pressed.
        @param keyEventRef - KeyEvent for pressed key
        @return true
        */
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	/** Checks if a key is released.
        @param keyEventRef - KeyEvent for released key
        @return true
        */
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
	/** Checks if mouse has moved and adjust camera.
        @param evt - MouseEvent
        @return true
        */
	bool mouseMoved(const OIS::MouseEvent &evt);
	/** Checks if a mouse button has been clicked and which.
        @param evt - MouseEvent
		@param id - the ID of the pressed button
        @return true
        */
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	/** Checks if a mouse button has been released and which.
        @param evt - MouseEvent
		@param id - the ID of the pressed button
        @return true
        */
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	
	Ogre::Root*					m_pRoot;
	Ogre::SceneManager*			m_pSceneMgr;
	Ogre::RenderWindow*			m_pRenderWnd;
	Ogre::Camera*				m_pCamera;
	Ogre::Viewport*				m_pViewport;
	Ogre::Log*					m_pLog;
	Ogre::Timer*				m_pTimer;
	
	OIS::InputManager*			m_pInputMgr;
	OIS::Keyboard*				m_pKeyboard;
	OIS::Mouse*					m_pMouse;

private:
	OgreFramework(const OgreFramework&);
	OgreFramework& operator= (const OgreFramework&);

	Ogre::Overlay*				m_pDebugOverlay;
	Ogre::Overlay*				m_pInfoOverlay;
	int							m_iNumScreenShots;

	bool						m_bShutDownOgre;
	
	Ogre::Vector3				m_TranslateVector;
	Ogre::Real					m_MoveSpeed; 
	Ogre::Degree				m_RotateSpeed; 
	float						m_MoveScale; 
	Ogre::Degree				m_RotScale;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif 

//|||||||||||||||||||||||||||||||||||||||||||||||