#ifndef OGRE_DEMO_HPP
#define OGRE_DEMO_HPP


#include "OgreFramework.hpp"

/** @brief Demo class used for showing the use of our extensions and the framework.

    @author Group 2
    */
class DemoApp : public OIS::KeyListener
{
public:
	/** Default constructor. Sets some default values.
        */
	DemoApp();
	/** Default destructor.
        */
	~DemoApp();

	/** Starts the demo by creating a framework, setting up the DemoScene and starting the main loop.
        */
	void startDemo();
	
	
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

	/** The method that the A* worker thread needs to be able to run.
        @param iparam - void pointer
        @return an unsigned long
        */
	static DWORD WINAPI threadStart(LPVOID iparam);
	static vector<vector<AstarNode*>> graphMapTemp;/**<The temporary vector with the graph map*/
	

private:
	/** Do the basic setup for the DemoScene.
        */
	void setupDemoScene();
	/** Starts the demo and the main loop.
        */
	void runDemo();
	/** Handle the logic and physics.
        */
	void handlePhysics();
	/** Initsialize the physics.
        */
	void initPhysics();
	vector<MovableObject*> getEntities();
	vector<float> getBoxValues(vector<MovableObject*> entityList);
	vector<vector<float>> getBoxPositions(vector<MovableObject*> entityList);

	//AI
	COORD astarDestination;
	bool nextLocation();
	void moveAstar(int timeSinceLastFrame);
	void initAstar();
	void newAstar();
	vector<vector<AstarNode*>> graphMap; /**<The vector with graph map*/
	Entity *mEntity;                 /**<The Entity we are animating*/
    SceneNode *mNode;                /**<The SceneNode that the Entity is attached to*/
    std::deque<Vector3> mWalkList;   /**<The list of points we are walking to*/
	AnimationState *mAnimationState;
    Real mWalkSpeed;   
	Real mDistance;                  /**<The distance the object has left to travel*/
    Vector3 mDirection;              /**<The direction the object is moving*/
    Vector3 mDestination;            /**<The destination the object is moving towards*/
	bool threadStarted;
	void setNotWalkables();
	int cdAstar;
	HANDLE thread;


	//The Soundplayer
	Soundplayer play;

	//The Projectile Cannon
	ProjectileCannon* cannon; 

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
	bool gren, shell, fShell, obliterate;
	int ID;

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