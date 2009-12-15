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

	//The soundplayer
	Soundplayer play;
	OGRE3DBody* mCharacter;
	Ogre::SceneNode*			m_pCubeNode;
	Ogre::Entity*				m_pCubeEntity;

	bool						m_bShutdown;
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif 

//|||||||||||||||||||||||||||||||||||||||||||||||