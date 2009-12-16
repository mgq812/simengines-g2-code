//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_DEMO_HPP
#define OGRE_DEMO_HPP

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "OgreFramework.hpp"

//|||||||||||||||||||||||||||||||||||||||||||||||

class DemoApp : public OIS::KeyListener
{
public:
	DemoApp();
	~DemoApp();

	void startDemo();
	
	

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);

private:
	void setupDemoScene();
	void runDemo();
	void handlePhysics();
	void initPhysics();

	//The Soundplayer
	Soundplayer play;

	//The Physics components
	NxOgre::World*		mWorld;
	NxOgre::Scene*		mScene;
	NxOgre::TimeController*	mTimeController;
	NxOgre::SceneDescription sceneDesc;
	OGRE3DRenderSystem*	mRenderSystem;
	OGRE3DKinematicBody* mCharacter;

	//The Ogre components
	Ogre::SceneNode*			m_pCubeNode;
	Ogre::Entity*				m_pCubeEntity;
	Ogre::Root*					root;
	Ogre::SceneManager*			sceneMgr;
	Ogre::Camera*				camera;
	
	//The OIS components
	OIS::Keyboard*				keyboard;
	OIS::Mouse*					mouse;

	double timeSinceLastFrame;
	double startTime;
	double timeSinceLastAction;

	bool						m_bShutdown;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif 

//|||||||||||||||||||||||||||||||||||||||||||||||