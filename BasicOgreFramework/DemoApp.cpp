//|||||||||||||||||||||||||||||||||||||||||||||||

#include "DemoApp.hpp"

#include <OgreLight.h>
#include <OgreWindowEventUtilities.h>

#include "math.h"
//|||||||||||||||||||||||||||||||||||||||||||||||

vector<vector<AstarNode*>> DemoApp::graphMapTemp;
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
	//Easy to use variables
	root = OgreFramework::getSingletonPtr()->m_pRoot;
	camera = OgreFramework::getSingletonPtr()->m_pCamera;
	sceneMgr = OgreFramework::getSingletonPtr()->m_pSceneMgr;
	keyboard = OgreFramework::getSingletonPtr()->m_pKeyboard;
	mouse = OgreFramework::getSingletonPtr()->m_pMouse;

	//Create the basic physic components
	
	initPhysics();
	
	

	//The sky system
	cartoon = new CartoonCaelum::CartoonSystem(root, sceneMgr, camera);
	//cartoon->addSnow();
	cartoon->addWindVector(Vector3(20, 0, 0));
	cartoon->addRain();
	//cartoon->setSnowDensity(100);
	//cartoon->setRainDensity(100);
	//cartoon->setRainVelocity(200, 230);
	root->addFrameListener(cartoon);
	//Crosshair
	Ogre::Overlay* crosshair = Ogre::OverlayManager::getSingleton().getByName("Crosshair/Overlay");
	crosshair->show();

	//Create light
	sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	sceneMgr->setShadowColour( ColourValue(0.5, 0.5, 0.5));
	sceneMgr->setAmbientLight( ColourValue(0.2, 0.2, 0.2));
	//Ogre::Light* light = sceneMgr->createLight("Light");
	//light->setPosition(5,10,5);
	//light->setType(Light::LT_POINT);
	//light->setDiffuseColour(1.0, 0, 0);
	//light->setSpecularColour(1.0, 0, 0);
	//light->setVisible(true);

	//Creating the character	
	mCharacter = mRenderSystem->createKinematicBody(new NxOgre::Box(1,5,1), NxOgre::Vec3(0,3.5f,0), "fish.mesh");
	mCharacter->getEntity()->setVisible(false);


	//Creating a fish
	m_pCubeEntity=OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("2","ogreHead.mesh");
	m_pCubeNode=OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode3",Vector3(0.0f,0.0f,500));
	m_pCubeNode->attachObject(m_pCubeEntity);

	//Creating a fish
	m_pCubeEntity=OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("3","ogreHead.mesh");
	m_pCubeNode=OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode24",Vector3(0.0f,0.0f,500));
	m_pCubeNode->attachObject(m_pCubeEntity);

	//Creating a fish
	m_pCubeEntity=OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("4","ogreHead.mesh");
	m_pCubeNode=OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode25",Vector3(0.0f,0.0f,500));
	m_pCubeNode->attachObject(m_pCubeEntity);

		//Creating a fish
	m_pCubeEntity = sceneMgr->createEntity("1","fish.mesh");
	m_pCubeNode=OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode2",Vector3(0.0f,m_pCubeEntity->getBoundingRadius(),0));
	m_pCubeNode->attachObject(m_pCubeEntity);

	play.setScales(2.0f, 2000000.0f);

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
	//Create the basic AI components
	initAstar();
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

			keyboard->capture();
			mouse->capture();

			OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
			OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();

			timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
			timeSinceLastAction += timeSinceLastFrame;			

			//play.setListenerPosition(OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->getAttachedObject("0")->getParentSceneNode()->getPosition().x, OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->getAttachedObject("0")->getParentSceneNode()->getPosition().y, OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->getAttachedObject("0")->getParentSceneNode()->getPosition().z);
			//Take care of the AI and its movements
			mAnimationState->addTime(timeSinceLastFrame/1000);
			moveAstar(timeSinceLastFrame);
			play.setListenerPosition(((float)mCharacter->getGlobalPosition().x), ((float)mCharacter->getGlobalPosition().y), ((float)mCharacter->getGlobalPosition().z));
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

//Key pressed
bool DemoApp::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
	
	if(keyboard->isKeyDown(OIS::KC_F))
	{
		//Find all entities and send their needed input into the playSoundWithEcho() method
		vector<int> boxValues;
		vector<vector<float>> boxPositions;
		vector<SceneNode*> allSceneNodes;
		vector<MovableObject*> entities;
		Ogre::SceneNode::ChildNodeIterator cNI = sceneMgr->getRootSceneNode()->getChildIterator();
		vector<string> names;
		while(cNI.hasMoreElements())
		{
			allSceneNodes.push_back(sceneMgr->getSceneNode(cNI.getNext()->getName()));
		}
		for(int i = 0; i < allSceneNodes.size(); i++)
		{
			Ogre::SceneNode::ObjectIterator cOI = allSceneNodes[i]->getAttachedObjectIterator();
			while(cOI.hasMoreElements())
			{
				Ogre::MovableObject* objectInput = cOI.getNext();
				if(objectInput->getName() != "snowSystem" && objectInput->getName()!= "rainSystem")
					entities.push_back(objectInput);
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
	if(keyboard->isKeyDown(OIS::KC_G))
	{
		play.playSound(2,1);
	}
	if(keyboard->isKeyDown(OIS::KC_H))
	{
		play.playSound(1,1);
	}
	if(keyboard->isKeyDown(OIS::KC_T))
	{
		static int option = 0;
		
		if(option == 0)
		{
			cartoon->addRain();
			option = 1;
		}
		else if(option == 1)
		{
			cartoon->removeRain();
			cartoon->addSnow();
			option = 2;
		}
		else if(option == 2)
		{
			cartoon->removeSnow();
			option = 0;
		}
	}
	if(keyboard->isKeyDown(OIS::KC_Y))
	{
		static int density = 0;
		
		if(density == 0)
		{
			cartoon->setSnowDensity(35);
			cartoon->setRainDensity(35);
			density = 1;
		}
		else if(density == 1)
		{
			cartoon->setSnowDensity(50);
			cartoon->setRainDensity(50);
			density = 2;
		}
		else if(density == 2)
		{
			cartoon->setSnowDensity(100);
			cartoon->setRainDensity(100);
			density = 0;
		}
	}

	return true;
}

//Key released
bool DemoApp::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
	
	return true;
}

//Initializes the physics components
void DemoApp::initPhysics()
{
	mWorld = NxOgre::World::createWorld();
	
	sceneDesc.mGravity = NxOgre::Vec3(0, -9.8f, 0);
	sceneDesc.mName = "BloodyMessTutorial2";

	mScene = mWorld->createScene(sceneDesc);

	//Set some physical scene values
	mScene->getMaterial(0)->setStaticFriction(0.5);
	mScene->getMaterial(0)->setDynamicFriction(0.5);
	mScene->getMaterial(0)->setRestitution(0);
	mRenderSystem = new OGRE3DRenderSystem(mScene);
	mTimeController = NxOgre::TimeController::getSingleton();

	//Create a plane
	mScene->createSceneGeometry(new NxOgre::PlaneGeometry(0, NxOgre::Vec3(0, 1, 0)));
	

	//MovablePlane *plane = new MovablePlane("Plane");
	//plane->d = 0;
	//plane->normal = Vector3::UNIT_Y;
	//Ogre::MeshManager::getSingleton().createPlane("PlaneMesh", 
	//	ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
	//	*plane, 120, 120, 1, 1, true, 1, 3, 3, Vector3::UNIT_Z);
	//Entity *planeEnt = sceneMgr->createEntity("PlaneEntity", "PlaneMesh");
	//Ogre::SceneNode* mPlaneNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
	//mPlaneNode->attachObject(planeEnt);
	//mPlaneNode->scale(100, 100, 100);
	//planeEnt->setMaterialName("Examples/GrassFloor");
	//planeEnt->setCastShadows(false);
	//mPlaneNode->getMaterial()->setReceiveShadows(true);
	//sceneMgr->setAmbientLight(ColourValue(0,0,0));
	Entity *ent;
	Plane plane(Vector3::UNIT_Y, 0);
	MeshManager::getSingleton().createPlane("ground",
		ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		1500,1500,20,20,true,1,200,200,Vector3::UNIT_Z);
	ent = sceneMgr->createEntity("GroundEntity", "ground");

	sceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ent);
	ent->setMaterialName("Examples/GrassFloor");
	ent->setCastShadows(false);




	//Add objects
	//mKB = mRenderSystem->createKinematicBody(new NxOgre::Box(12,1,12), NxOgre::Vec3(0, 12, 0), "cube.mesh");
	OGRE3DBody* gCube;
	float x_c, y_c, x_old, y_old;
	float aMod = 0;
	float sMod = 0;
	float step = 0;
	float dist; 
	x_old = 100;
	y_old = 100;
	for(int cnt = 1; cnt < 200; cnt++) 
	{
		aMod = 3;
		sMod = 6;
		step = (2*3.14/200)*cnt  ;
		x_c =  aMod*(sMod*cos(step) + cos(sMod*(float)step));
		y_c =  aMod*(sMod*sin(step) + sin(sMod*(float)step));
		dist = sqrt(pow(x_c - x_old, 2) + pow(y_c - y_old, 2));
		if(dist > 2.1) 
		{
			gCube = mRenderSystem->createBody(new NxOgre::Box(1,1,1), NxOgre::Vec3(x_c, 2, y_c), "cube.1m.mesh");
			gCube->setMass(2);
			gCube->getEntity()->setCastShadows(true);
			x_old = x_c;
			y_old = y_c;
		}
	}
	cannon = new ProjectileCannon(mRenderSystem, camera->getDirection(), camera->getPosition());
	ID = cannon->addLauncher(camera->getDirection(), camera->getPosition());
}

//Handle the physics each update
void DemoApp::handlePhysics()
{
	NxOgre::TimeController::getSingleton()->advance(timeSinceLastFrame/1000);
	cannon->purge(timeSinceLastFrame/1000);
	NxOgre::Vec3 moveTo = NxVec3(0,0,0);
	
	//Move camera to the character
	camera->setPosition(mCharacter->getGlobalPosition().x, mCharacter->getGlobalPosition().y, mCharacter->getGlobalPosition().z);
	
	//Jump
	if(keyboard->isKeyDown(OIS::KC_Q))
	{
		//mCharacter->addForce(NxOgre::Vec3(0,10,0), NxOgre::Enums::ForceMode_Impulse);
	}

	//Fire projectile
	if(keyboard->isKeyDown(OIS::KC_SPACE) && timeSinceLastAction > 200)
	{
		cannon->aimCannon(camera->getDirection(), ID);
		cannon->moveCannon(camera->getPosition(), ID);
		cannon->fireCannon(1, ID);
		//cannon->fireFastShell(ID);
		timeSinceLastAction = 0;
	}

	//Movements
	if(keyboard->isKeyDown(OIS::KC_W))
	{
		moveTo.x = mCharacter->getGlobalPosition().x + (camera->getDirection().x / camera->getDirection().normalise())*timeSinceLastFrame/50;
		moveTo.y = mCharacter->getGlobalPosition().y;
		moveTo.z = mCharacter->getGlobalPosition().z + (camera->getDirection().z / camera->getDirection().normalise())*timeSinceLastFrame/50;
		
		mCharacter->setGlobalPosition(moveTo);
	}
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
	{
		camera->yaw(Ogre::Radian(Math::PI/2));
		moveTo.x = mCharacter->getGlobalPosition().x + (camera->getDirection().x / camera->getDirection().normalise())*timeSinceLastFrame/50;	
		moveTo.y = mCharacter->getGlobalPosition().y;
		moveTo.z = mCharacter->getGlobalPosition().z + (camera->getDirection().z / camera->getDirection().normalise())*timeSinceLastFrame/50;
		camera->yaw(Ogre::Radian((-Math::PI/2)));
		mCharacter->setGlobalPosition(moveTo);
	}
	if(keyboard->isKeyDown(OIS::KC_S))
	{
		moveTo.x = mCharacter->getGlobalPosition().x - (camera->getDirection().x / camera->getDirection().normalise())*timeSinceLastFrame/50;
		moveTo.y = mCharacter->getGlobalPosition().y;
		moveTo.z = mCharacter->getGlobalPosition().z - (camera->getDirection().z / camera->getDirection().normalise())*timeSinceLastFrame/50;

		mCharacter->setGlobalPosition(moveTo);
	}
	if(keyboard->isKeyDown(OIS::KC_D))
	{
		camera->yaw(Ogre::Radian((-Math::PI/2)));
		moveTo.x = mCharacter->getGlobalPosition().x + (camera->getDirection().x / camera->getDirection().normalise())*timeSinceLastFrame/50;	
		moveTo.y = mCharacter->getGlobalPosition().y;
		moveTo.z = mCharacter->getGlobalPosition().z + (camera->getDirection().z / camera->getDirection().normalise())*timeSinceLastFrame/50;
		camera->yaw(Ogre::Radian(Math::PI/2));
		mCharacter->setGlobalPosition(moveTo);
	}
}

void DemoApp::initAstar(){
	astarDestination.X = 0;
	astarDestination.Y = 0;
	
	mNode = m_pCubeNode;
	mEntity = m_pCubeEntity;
	mWalkSpeed = 20.0f;
	mAnimationState = mEntity->getAnimationState("swim");
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(true);
	
	graphMap = Astar::GenerateGraphMap(20);
	threadStarted = false;
	newAstar();
}

DWORD WINAPI DemoApp::threadStart(LPVOID iparam){
	//DemoApp* da = (DemoApp*)iparam;
	DemoApp::graphMapTemp = Astar::GenerateGraphMap(20);
	return 0;
}

void DemoApp::newAstar(){
	
	//	graphMap = Astar::GenerateGraphMap(20);
	//Sleep(1000);
	mDirection = Vector3::ZERO;
	COORD temp = astarDestination;
	astarDestination.X = rand()%20;
	while(astarDestination.X > temp.X +5 || astarDestination.X < temp.X -5)
	{
		astarDestination.X = rand()%20;
	}
	astarDestination.Y = rand()%20;
	while(astarDestination.Y > temp.Y +5 || astarDestination.Y < temp.Y -5)
	{
		astarDestination.Y = rand()%20;
	}
	if(threadStarted){
		WaitForSingleObject(thread,INFINITE);
		graphMap = DemoApp::graphMapTemp;
		CloseHandle(thread);
	}
	vector<COORD> movementVector = Astar::GenerateAstarPath(*graphMap[temp.X][temp.Y],*graphMap[astarDestination.X][astarDestination.Y], graphMap);
	for(int i = 0;i < movementVector.size();i++){
		COORD temp = Astar::convertAstarToOgreCoords(movementVector[i], 300, 20);
		mWalkList.push_back(Vector3(temp.X, mEntity->getBoundingRadius(), temp.Y));
	}
	threadStarted = true;
	thread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)threadStart,0,0,NULL);
}
void DemoApp::moveAstar(int timeSinceLastFrame){
	
	if (mDirection == Vector3::ZERO) 
	{
		if (nextLocation()) 
		{
			mAnimationState = mEntity->getAnimationState("swim");
			mAnimationState->setLoop(true);
			mAnimationState->setEnabled(true);
		}
		else
		{
			newAstar();
		}
	}
	else
	{
		Real move = mWalkSpeed * timeSinceLastFrame/1000;
		mDistance -= move;
		if (mDistance <= 0.0f)
		{
			mNode->setPosition(mDestination);
			mDirection = Vector3::ZERO;
			// Set animation based on if the robot has another point to walk to. 
			if (! nextLocation())
			{
				//newAstar();
			} 
			else
			{
				Vector3 src = mNode->getOrientation() * Vector3::UNIT_X;
				if ((1.0f + src.dotProduct(mDirection)) < 0.0001f)
				{
					mNode->yaw(Degree(180));
				}
				else
				{
					Ogre::Quaternion quat = src.getRotationTo(mDirection);
					mNode->rotate(quat);
					//mNode->yaw(Degree(180));
				} // else
				mNode->yaw(Degree(180));
			}
		}
		else
		{
			mNode->translate(mDirection * move);
		} // else
	} // if
}
bool DemoApp::nextLocation(){
	if (mWalkList.empty())
            return false;
	mDestination = mWalkList.front();
    mWalkList.pop_front();       
	//Lacking mnode atm
    mDirection = mDestination - mNode->getPosition();
    mDistance = mDirection.normalise();
	return true;
}
//|||||||||||||||||||||||||||||||||||||||||||||||