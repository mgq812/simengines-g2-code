//|||||||||||||||||||||||||||||||||||||||||||||||

#include "DemoApp.hpp"

#include <OgreLight.h>
#include <OgreWindowEventUtilities.h>

#include "math.h"
#include "Crosshair.h"
//|||||||||||||||||||||||||||||||||||||||||||||||


int ID;
ProjectileCannon* cannon; 

DemoApp::DemoApp()
{
	m_pCubeNode			= 0;
	m_pCubeEntity		= 0;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

DemoApp::~DemoApp()
{
	delete OgreFramework::getSingletonPtr();
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void DemoApp::startDemo()
{
	new OgreFramework();
	OgreFramework::getSingletonPtr()->initOgre("DemoApp v1.0", this, 0);
	
	m_bShutdown = false;

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Demo initialized!");

	setupDemoScene();
	runDemo();
}

//|||||||||||||||||||||||||||||||||||||||||||||||
void DemoApp::setupDemoScene()
{
	int i = 12;
	NxOgre::World*		mWorld;
	NxOgre::Scene*		mScene;
	NxOgre::TimeController*	mTimeController;
	OGRE3DRenderSystem*	mRenderSystem;
	mWorld = NxOgre::World::createWorld();
	
	NxOgre::SceneDescription sceneDesc;
	sceneDesc.mGravity = NxOgre::Vec3(0, -9.8f, 0);
	sceneDesc.mName = "BloodyMessTutorial2";

	mScene = mWorld->createScene(sceneDesc);

	// Set some physical scene values
	mScene->getMaterial(0)->setStaticFriction(0.5);
	mScene->getMaterial(0)->setDynamicFriction(0.5);
	mScene->getMaterial(0)->setRestitution(0);
	mRenderSystem = new OGRE3DRenderSystem(mScene);
	mTimeController = NxOgre::TimeController::getSingleton();
	//create a plane
	mScene->createSceneGeometry(new NxOgre::PlaneGeometry(0, NxOgre::Vec3(0, 1, 0)));
	

	MovablePlane *plane = new MovablePlane("Plane");
	plane->d = 0;
	plane->normal = Vector3::UNIT_Y;
	Ogre::MeshManager::getSingleton().createPlane("PlaneMesh", 
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
		*plane, 120, 120, 1, 1, true, 1, 3, 3, Vector3::UNIT_Z);
	Entity *planeEnt = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("PlaneEntity", "PlaneMesh");
	planeEnt->setMaterialName("Examples/GrassFloor");

	Ogre::SceneNode* mPlaneNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode();
	mPlaneNode->attachObject(planeEnt);
	mPlaneNode->scale(100, 100, 100);

	// Add objects
	//mKB = mRenderSystem->createKinematicBody(new NxOgre::Box(12,1,12), NxOgre::Vec3(0, 12, 0), "cube.mesh");
	OGRE3DBody* gCube;
	float x_c, y_c, x_old, y_old;
	float aMod = 0;
	float sMod = 0;
	float step = 0;
	float dist; 
	x_old = 100;
	y_old = 100;
	for(int cnt = 1; cnt < 200; cnt++) {
		aMod = 3;
		sMod = 6;
		step = (2*3.14/200)*cnt  ;
		x_c =  aMod*(sMod*cos(step) + cos(sMod*(float)step));
		y_c =  aMod*(sMod*sin(step) + sin(sMod*(float)step));
		dist = sqrt(pow(x_c - x_old, 2) + pow(y_c - y_old, 2));
		if(dist > 2.1) {
			gCube = mRenderSystem->createBody(new NxOgre::Box(1,1,1), NxOgre::Vec3(x_c, 14, y_c), "cube.1m.mesh");
			gCube->setMass(2);
			x_old = x_c;
			y_old = y_c;
		}
	}
	cannon = new ProjectileCannon(mRenderSystem, OgreFramework::getSingletonPtr()->m_pCamera->getDirection(), OgreFramework::getSingletonPtr()->m_pCamera->getPosition());
	ID = cannon->addLauncher(OgreFramework::getSingletonPtr()->m_pCamera->getDirection(), OgreFramework::getSingletonPtr()->m_pCamera->getPosition());
	
	//The sky system
	CartoonCaelum::CartoonSystem* cartoon = 
		new CartoonCaelum::CartoonSystem(OgreFramework::getSingletonPtr()->m_pRoot, 
		OgreFramework::getSingletonPtr()->m_pSceneMgr, OgreFramework::getSingletonPtr()->m_pCamera);
	cartoon->addSnow();
	cartoon->addWindVector(Vector3(20, 0, 0));
	cartoon->setSnowDensity(100);
	cartoon->addRain();
	OgreFramework::getSingletonPtr()->m_pRoot->addFrameListener(cartoon);

	//crosshair
	Ogre::Overlay* crosshair = Ogre::OverlayManager::getSingleton().getByName("Crosshair/Overlay");
	crosshair->show();

	vector<vector<AstarNode*>> graphMap = Astar::GenerateGraphMap(1);
	//OgreFramework::getSingletonPtr()->m_pSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
	//OgreFramework::getSingletonPtr()->m_pSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
	//OgreFramework::getSingletonPtr()->m_pSceneMgr->setWorldGeometry("terrain.cfg");

	//Create light
	OgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Light")->setPosition(15,60,15);
	OgreFramework::getSingletonPtr()->m_pSceneMgr->setShadowTechnique(SHADOWTYPE_TEXTURE_ADDITIVE);
	

	//Creating the character	
	mCharacter = mRenderSystem->createBody(new NxOgre::Box(10,10,10), NxOgre::Vec3(0,20,0), "fish.mesh");
	mCharacter->setMass(80);
	mCharacter->getEntity()->setVisible(false);
	//create ze ogrehead
	m_pCubeEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("0", "ogrehead.mesh");
	m_pCubeNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode", Vector3(100.0f, 0.0f, 200));
	m_pCubeNode->attachObject(m_pCubeEntity);

	//Creating a fish
	m_pCubeEntity=OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("1","fish.mesh");
	m_pCubeNode=OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode2",Vector3(0.0f,0.0f,500));
	m_pCubeNode->attachObject(m_pCubeEntity);

	play.addSound("..\\..\\Extensions\\Sound\\cat.wav", 0,0,0,0,0,0);
	play.addSound("..\\..\\Extensions\\Sound\\Will.wav", 0,0,0,0,0,0);
	play.addSound("..\\..\\Extensions\\Sound\\Worry.wav", 0,0,0,0,0,0);
	play.addSound("..\\..\\Extensions\\Sound\\Jungle.wav", 0,0,0,0,0,0);
	play.addSound("..\\..\\Extensions\\Sound\\asd.wav", 0,0,0,0,0,0);
	play.addSound("..\\..\\Extensions\\Sound\\asd2.wav", 0,0,0,0,0,0);
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void DemoApp::runDemo()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Start main loop...");
	
	timeSinceLastFrame = 0;
	startTime = 0;
	timeSinceLastAction = 0;
	OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();
	
	while(!m_bShutdown && !OgreFramework::getSingletonPtr()->isOgreToBeShutDown()) 
	{
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())m_bShutdown = true;

		#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
				Ogre::WindowEventUtilities::messagePump();
		#endif
		
		//The main loop
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{	
			startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();

			OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
			OgreFramework::getSingletonPtr()->m_pMouse->capture();

			OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
			OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();

			timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
			timeSinceLastAction += timeSinceLastFrame;

			play.setListenerPosition(OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->getAttachedObject("0")->getParentSceneNode()->getPosition().x, OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->getAttachedObject("0")->getParentSceneNode()->getPosition().y, OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->getAttachedObject("0")->getParentSceneNode()->getPosition().z);
			

			//Take care of the logic and movments
			handlePhysics();
		}
		else
		{
			Sleep(1000);
		}
	}

	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Main loop quit");
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutdown OGRE...");
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool DemoApp::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
	
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_F))
	{
		//Find all entities and send their needed input into the playSoundWithEcho() method
		vector<int> boxValues;
		vector<vector<float>> boxPositions;
		vector<SceneNode*> allSceneNodes;
		vector<MovableObject*> entities;
		Ogre::SceneNode::ChildNodeIterator cNI = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->getChildIterator();
		while(cNI.hasMoreElements())
		{
			allSceneNodes.push_back(OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode(cNI.getNext()->getName()));
		}
		for(int i = 0; i < allSceneNodes.size(); i++)
		{
			Ogre::SceneNode::ObjectIterator cOI = allSceneNodes[i]->getAttachedObjectIterator();
			while(cOI.hasMoreElements())
			{
				entities.push_back(cOI.getNext());
			}
		}
		for(int y = 0; y < entities.size(); y++)
		{
			boxValues.push_back(entities[y]->getBoundingBox().volume());
			
			vector<float> in;
			in.push_back(entities[y]->getParentSceneNode()->getPosition().x);
			in.push_back(entities[y]->getParentSceneNode()->getPosition().y);
			in.push_back(entities[y]->getParentSceneNode()->getPosition().z);
			boxPositions.push_back(in);
		}

		play.playSoundWithEcho(0, 1, boxValues, boxPositions);

	}
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_G))
	{
		play.playSound(2,1);
	}
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_H))
	{
		play.playSound(1,1);
	}
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_T))
	{
		if(OgreFramework::getSingletonPtr()->m_pSceneMgr->getEntity("0")->isVisible())
			OgreFramework::getSingletonPtr()->m_pSceneMgr->getEntity("0")->setVisible(false);
		else
			OgreFramework::getSingletonPtr()->m_pSceneMgr->getEntity("0")->setVisible(true);
	}

	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

bool DemoApp::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
	
	return true;
}

//|||||||||||||||||||||||||||||||||||||||||||||||

void DemoApp::handlePhysics()
{
	NxOgre::TimeController::getSingleton()->advance(timeSinceLastFrame/1000);
	cannon->purge(timeSinceLastFrame/1000);
	NxOgre::Vec3 moveTo = NxVec3(0,0,0);
	
	OgreFramework::getSingletonPtr()->m_pCamera->setPosition(mCharacter->getGlobalPosition().x, mCharacter->getGlobalPosition().y, mCharacter->getGlobalPosition().z);
	
	//Jump
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Q))
	{
		mCharacter->addForce(NxOgre::Vec3(0,10,0), NxOgre::Enums::ForceMode_Impulse);
	}

	//Fire projectile
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_SPACE) && timeSinceLastAction > 200)
	{
		cannon->aimCannon(OgreFramework::getSingletonPtr()->m_pCamera->getDirection(), ID);
		cannon->moveCannon(OgreFramework::getSingletonPtr()->m_pCamera->getPosition(), ID);
		cannon->fireFastShell(ID);
		timeSinceLastAction = 0;
	}

	//Movements
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
	{
		moveTo.x = mCharacter->getGlobalPosition().x + (OgreFramework::getSingletonPtr()->m_pCamera->getDirection().x / OgreFramework::getSingletonPtr()->m_pCamera->getDirection().normalise())*timeSinceLastFrame/50;
		moveTo.y = mCharacter->getGlobalPosition().y;
		moveTo.z = mCharacter->getGlobalPosition().z + (OgreFramework::getSingletonPtr()->m_pCamera->getDirection().z / OgreFramework::getSingletonPtr()->m_pCamera->getDirection().normalise())*timeSinceLastFrame/50;
		
		mCharacter->setGlobalPosition(moveTo);
	}
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
	{
		OgreFramework::getSingletonPtr()->m_pCamera->yaw(Ogre::Radian(Math::PI/2));
		moveTo.x = mCharacter->getGlobalPosition().x + (OgreFramework::getSingletonPtr()->m_pCamera->getDirection().x / OgreFramework::getSingletonPtr()->m_pCamera->getDirection().normalise())*timeSinceLastFrame/50;	
		moveTo.y = mCharacter->getGlobalPosition().y;
		moveTo.z = mCharacter->getGlobalPosition().z + (OgreFramework::getSingletonPtr()->m_pCamera->getDirection().z / OgreFramework::getSingletonPtr()->m_pCamera->getDirection().normalise())*timeSinceLastFrame/50;
		OgreFramework::getSingletonPtr()->m_pCamera->yaw(Ogre::Radian((-Math::PI/2)));
		mCharacter->setGlobalPosition(moveTo);
	}
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
	{
		moveTo.x = mCharacter->getGlobalPosition().x - (OgreFramework::getSingletonPtr()->m_pCamera->getDirection().x / OgreFramework::getSingletonPtr()->m_pCamera->getDirection().normalise())*timeSinceLastFrame/50;
		moveTo.y = mCharacter->getGlobalPosition().y;
		moveTo.z = mCharacter->getGlobalPosition().z - (OgreFramework::getSingletonPtr()->m_pCamera->getDirection().z / OgreFramework::getSingletonPtr()->m_pCamera->getDirection().normalise())*timeSinceLastFrame/50;

		mCharacter->setGlobalPosition(moveTo);
	}
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D))
	{
		OgreFramework::getSingletonPtr()->m_pCamera->yaw(Ogre::Radian((-Math::PI/2)));
		moveTo.x = mCharacter->getGlobalPosition().x + (OgreFramework::getSingletonPtr()->m_pCamera->getDirection().x / OgreFramework::getSingletonPtr()->m_pCamera->getDirection().normalise())*timeSinceLastFrame/50;	
		moveTo.y = mCharacter->getGlobalPosition().y;
		moveTo.z = mCharacter->getGlobalPosition().z + (OgreFramework::getSingletonPtr()->m_pCamera->getDirection().z / OgreFramework::getSingletonPtr()->m_pCamera->getDirection().normalise())*timeSinceLastFrame/50;
		OgreFramework::getSingletonPtr()->m_pCamera->yaw(Ogre::Radian(Math::PI/2));
		mCharacter->setGlobalPosition(moveTo);
	}
}

//|||||||||||||||||||||||||||||||||||||||||||||||