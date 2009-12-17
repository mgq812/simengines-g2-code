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
	static DWORD WINAPI threadStart(LPVOID iparam);
	static vector<vector<AstarNode*>> graphMapTemp;

private:
	void setupDemoScene();
	void runDemo();
	void handlePhysics();
	void initPhysics();

	//AI
	COORD astarDestination;
	bool nextLocation();
	void moveAstar(int timeSinceLastFrame);
	void initAstar();
	void newAstar();
	vector<vector<AstarNode*>> graphMap;
	Entity *mEntity;                 // The Entity we are animating
    SceneNode *mNode;                // The SceneNode that the Entity is attached to
    std::deque<Vector3> mWalkList;   // The list of points we are walking to
	AnimationState *mAnimationState;
    Real mWalkSpeed;   
	Real mDistance;                  // The distance the object has left to travel
    Vector3 mDirection;              // The direction the object is moving
    Vector3 mDestination;            // The destination the object is moving towards
	bool threadStarted;
	void setNotWalkables();
	int cdAstar;
	HANDLE thread;


	//The Soundplayer
	Soundplayer play;

	//The Cartoon System
	CartoonCaelum::CartoonSystem* cartoon;

	//The Physics components
	NxOgre::World*		mWorld;
	NxOgre::Scene*		mScene;
	NxOgre::TimeController*	mTimeController;
	NxOgre::SceneDescription sceneDesc;
	OGRE3DRenderSystem*	mRenderSystem;
	OGRE3DKinematicBody* mCharacter;
	OGRE3DBody* theFish;
	bool gren, shell, fShell;

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