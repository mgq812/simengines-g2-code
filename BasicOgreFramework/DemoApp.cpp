//|||||||||||||||||||||||||||||||||||||||||||||||

#include "DemoApp.hpp"

#include <OgreLight.h>
#include <OgreWindowEventUtilities.h>

#include "math.h"
//|||||||||||||||||||||||||||||||||||||||||||||||

vector<vector<AstarNode*>> DemoApp::graphMapTemp;

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
	cartoon->addWindVector(Vector3(20, 0, 0));
	root->addFrameListener(cartoon);

	//Crosshair
	Ogre::Overlay* crosshair = Ogre::OverlayManager::getSingleton().getByName("Crosshair/Overlay");
	crosshair->show();

	//Create light
	sceneMgr->setShadowTechnique(SHADOWTYPE_STENCIL_ADDITIVE);
	sceneMgr->setShadowColour( ColourValue(0.5, 0.5, 0.5));
	sceneMgr->setAmbientLight( ColourValue(0.2, 0.2, 0.2));
	Ogre::Light* light = sceneMgr->createLight("Light");
	light->setPosition(30,20,5);
	light->setType(Light::LT_POINT);
	light->setDiffuseColour(1.0, 1.0, 1.0);
	light->setSpecularColour(1.0, 1.0, 1.0);
	light->setVisible(true);

	//Creating the character	
	mCharacter = mRenderSystem->createKinematicBody(new NxOgre::Box(1,5,1), NxOgre::Vec3(20,3.5f,20), "fish.mesh");
	mCharacter->getEntity()->setVisible(false);

	//Creating boulders
	OGRE3DKinematicBody* boulder;
	for(int i = -30; i < 0; i += 5)
	{
		boulder = mRenderSystem->createKinematicBody(new NxOgre::Box(5,7,5), NxOgre::Vec3(0.0f,0,i), "boulder_02.mesh");
	}
	OGRE3DKinematicBody* houseA = mRenderSystem->createKinematicBody(new NxOgre::Box(9,10,5), NxOgre::Vec3(4.0f,0,1.0f), "cg_house_A.mesh");
	OGRE3DKinematicBody* well = mRenderSystem->createKinematicBody(new NxOgre::Box(6,16,6), NxOgre::Vec3(2.0f,0,20.0f), "cg_well.mesh");
	well->getSceneNode()->scale(2.0f,2.0f,2.0f);
	OGRE3DKinematicBody* wella = mRenderSystem->createKinematicBody(new NxOgre::Box(6,16,6), NxOgre::Vec3(14.0f,0,20.0f), "cg_well.mesh");
	wella->getSceneNode()->scale(2.0f,2.0f,2.0f);
	OGRE3DKinematicBody* wellb = mRenderSystem->createKinematicBody(new NxOgre::Box(6,16,6), NxOgre::Vec3(24.0f,0,20.0f), "cg_well.mesh");
	wellb->getSceneNode()->scale(2.0f,2.0f,2.0f);

	//Creating a robot and let it be the last one created
	m_pCubeEntity = sceneMgr->createEntity("1","robot.mesh");
	m_pCubeNode=OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode2",Vector3(20.0f,0.0f,10.0f));
	m_pCubeNode->attachObject(m_pCubeEntity);
	m_pCubeNode->scale(0.05f,0.05f,0.05f);

	//Create the basic AI components
	initAstar();

	//Initialize the sound player and add the sounds later to be played
	play.setScales(1.8f, 400000.0f);

	play.addSound("..\\..\\Extensions\\Sound\\cat.wav", 0,0,0,0,0,0);
	play.addSound("..\\..\\Extensions\\Sound\\Grenade3.wav", 0, 0, 0, 0, 0, 0);
	play.addSound("..\\..\\Extensions\\Sound\\bazooka3.wav", 0, 0, 0, 0, 0, 0);
	play.addSound("..\\..\\Extensions\\Sound\\roboto.wav", 0,0,0,0,0,0);
	play.addSound("..\\..\\Extensions\\Sound\\rain.wav", 0,0,0,0,0,0);

	//Starts a looping song
	play.playSound(3, 0.2f, true);


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

			keyboard->capture();
			mouse->capture();

			OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
			OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();

			timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
			timeSinceLastAction += timeSinceLastFrame;			

			//Take care of the AI and its movements
			mAnimationState->addTime(timeSinceLastFrame/1000);
			moveAstar(timeSinceLastFrame);

			//Set the listener position to the characters position
			play.setListenerPosition(((float)mCharacter->getGlobalPosition().x), ((float)mCharacter->getGlobalPosition().y), ((float)mCharacter->getGlobalPosition().z));
			play.setSourcePosition(0, ((float)mCharacter->getGlobalPosition().x), ((float)mCharacter->getGlobalPosition().y), ((float)mCharacter->getGlobalPosition().z));
			play.setSourcePosition(3, ((float)mNode->getPosition().x), ((float)mNode->getPosition().y), ((float)mNode->getPosition().z));
			play.setSourcePosition(4, ((float)mCharacter->getGlobalPosition().x), ((float)mCharacter->getGlobalPosition().y), ((float)mCharacter->getGlobalPosition().z));

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
		vector<MovableObject*> eList = getEntities();
		play.playSoundWithEcho(0, 1, getBoxValues(eList), getBoxPositions(eList));

	}
	if(keyboard->isKeyDown(OIS::KC_G))
	{
		play.playSound(2,1, false);
	}
	if(keyboard->isKeyDown(OIS::KC_H))
	{
		play.playSound(1,1, false);
	}
	//Change between the different weathers; rain, snow and clear
	if(keyboard->isKeyDown(OIS::KC_T))
	{
		static int option = 0;

		if(option == 0)
		{
			cartoon->addRain();
			play.playSound(4, 1.0f, true);
			option = 1;
		}
		else if(option == 1)
		{
			cartoon->removeRain();
			play.stopSound(4);
			cartoon->addSnow();
			option = 2;
		}
		else if(option == 2)
		{
			cartoon->removeSnow();
			option = 0;
		}
	}
	//Change the density of the weather effects
	if(keyboard->isKeyDown(OIS::KC_Y))
	{
		static int density = 0;

		if(density == 0)
		{
			cartoon->setSnowDensity(20);
			cartoon->setRainDensity(20);
			density = 1;
		}
		else if(density == 1)
		{
			cartoon->setSnowDensity(70);
			cartoon->setRainDensity(70);
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
	mScene->getMaterial(0)->setStaticFriction(1.0);
	mScene->getMaterial(0)->setDynamicFriction(1.0);
	mScene->getMaterial(0)->setRestitution(0);
	mRenderSystem = new OGRE3DRenderSystem(mScene);
	mTimeController = NxOgre::TimeController::getSingleton();

	//Create a plane
	mScene->createSceneGeometry(new NxOgre::PlaneGeometry(0, NxOgre::Vec3(0, 1, 0)));

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
	OGRE3DBody* gCube;

	for(int p = 2; p < 7; p++)
		for(int h = 0; h < 2; h++) 
			for(int i = 0; i < 2; i++) {
				for(int j = 0; j < 2; j++) { 
					gCube = mRenderSystem->createBody(new NxOgre::Box(1,1,1), NxOgre::Vec3(20*p+ i, 1+h+0.001f*h, 20*p+j), "cube.1m.mesh");
					gCube->setMass(2);
					gCube->getEntity()->setCastShadows(true);
					gCube->setForceFieldMaterial(0);
					gCube->setAngularDamping(0.5);
				}
			}
			cannon = new ProjectileCannon(mRenderSystem);
			ID = cannon->addLauncher(camera->getDirection(), camera->getPosition());

			//default cannon values, for launching
			gren = true;
			shell = false;
			fShell = false;
			obliterate = false; 
}

//Handle the physics each update
void DemoApp::handlePhysics()
{
	NxOgre::TimeController::getSingleton()->advance(timeSinceLastFrame/1000);
	cannon->purge(timeSinceLastFrame/1000);
	NxOgre::Vec3 moveTo = NxVec3(0,0,0);

	//Move camera to the character
	camera->setPosition(mCharacter->getGlobalPosition().x, mCharacter->getGlobalPosition().y, mCharacter->getGlobalPosition().z);

	//Fire projectile
	if(keyboard->isKeyDown(OIS::KC_SPACE) && timeSinceLastAction > 200 || mouse->getMouseState().buttonDown(OIS::MB_Left) && timeSinceLastAction > 200)
	{
		moveTo.x = mCharacter->getGlobalPosition().x + (camera->getDirection().x / camera->getDirection().normalise())*10;
		moveTo.y = mCharacter->getGlobalPosition().y;
		moveTo.z = mCharacter->getGlobalPosition().z + (camera->getDirection().z / camera->getDirection().normalise())*10;
		cannon->aimCannon(camera->getDirection(), ID);
		cannon->moveCannon(moveTo, ID);
		if(shell)
		{
			cannon->fireShell(ID);

			vector<MovableObject*> eList = getEntities();
			play.setSourcePosition(2, mCharacter->getGlobalPosition().x, mCharacter->getGlobalPosition().y, mCharacter->getGlobalPosition().z);
			play.playSoundWithEcho(2, 1.0f, getBoxValues(eList), getBoxPositions(eList));
		}
		else if(fShell)
		{
			cannon->fireFastShell(ID);

			vector<MovableObject*> eList = getEntities();
			play.setSourcePosition(2, mCharacter->getGlobalPosition().x, mCharacter->getGlobalPosition().y, mCharacter->getGlobalPosition().z);
			play.playSoundWithEcho(2,1.0f, getBoxValues(eList), getBoxPositions(eList));
		}
		else if(gren)
		{
			cannon->fireGrenade(ID);

			play.setSourcePosition(1, mCharacter->getGlobalPosition().x, mCharacter->getGlobalPosition().y, mCharacter->getGlobalPosition().z);
			vector<MovableObject*> eList = getEntities();
			play.playWithEchoIn(1, 1.0f, cannon->getGreandeLife()*1000, getBoxValues(eList), getBoxPositions(eList));
		}
		if(obliterate)
		{
			cannon->obliterate(ID);
		}
		timeSinceLastAction = 0;
	}
	if(keyboard->isKeyDown(OIS::KC_1)) { 
		gren = true;
		shell = false;
		fShell = false;
		obliterate = false;
	}
	if(keyboard->isKeyDown(OIS::KC_2)) {
		gren = false;
		shell = true;
		fShell = false;		
		obliterate = false;
	}
	if(keyboard->isKeyDown(OIS::KC_3)) {
		gren = false;
		shell = false;
		fShell = true;
		obliterate = false;
	}
	if(keyboard->isKeyDown(OIS::KC_4)) {
		gren = false;
		shell = false;
		fShell = false;
		obliterate = true;
	}
	//Movements; forward, left, backwards and right
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
	astarDestination.X = 16;
	astarDestination.Y = 13;
	cdAstar = 5000;

	mNode = m_pCubeNode;
	mEntity = m_pCubeEntity;
	mWalkSpeed = 2.0f;
	mAnimationState = mEntity->getAnimationState("Walk");
	mAnimationState->setLoop(true);
	mAnimationState->setEnabled(true);


	graphMap = Astar::GenerateGraphMap(30);
	threadStarted = false;
	newAstar();
}

DWORD WINAPI DemoApp::threadStart(LPVOID iparam){
	DemoApp::graphMapTemp = Astar::GenerateGraphMap(30);
	return 0;
}

void DemoApp::newAstar()
{
	mDirection = Vector3::ZERO;
	COORD temp = astarDestination;
	NxOgre::Vec3 playerPos = mCharacter->getGlobalPosition();
	if(playerPos.x > 0)
	{
		astarDestination.X = playerPos.x + 0.5;
	}
	else
	{
		astarDestination.X = playerPos.x - 0.5;
	}
	if(playerPos.z > 0)
	{
		astarDestination.Y = playerPos.z + 0.5;
	}
	else
	{
		astarDestination.Y = playerPos.z - 0.5;
	}
	astarDestination = Astar::convertOgreToAstarCoords(astarDestination,100,30);
	if((temp.X != astarDestination.X || temp.Y != astarDestination.Y) && 
		((astarDestination.X < 30 && astarDestination.X > 0) && (astarDestination.Y < 30 && astarDestination.Y > 0)) &&
		((temp.X < 30 && temp.X > 0) && (temp.Y < 30 && temp.Y > 0)))
	{
		if(threadStarted){
			WaitForSingleObject(thread,INFINITE);
			graphMap = DemoApp::graphMapTemp;
			CloseHandle(thread);
		}
		setNotWalkables();
		vector<COORD> movementVector = Astar::GenerateAstarPath(*graphMap[temp.X][temp.Y],*graphMap[astarDestination.X][astarDestination.Y], graphMap);
		if(movementVector.size() == 0)
		{
			astarDestination = temp;
		}
		for(int i = 0;i < movementVector.size();i++){
			COORD temp = Astar::convertAstarToOgreCoords(movementVector[i], 100, 30);
			mWalkList.push_back(Vector3(temp.X, 0, temp.Y));
		}
		threadStarted = true;
		thread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)threadStart,0,0,NULL);
	}
	else
	{
		astarDestination = temp;
	}
}
void DemoApp::moveAstar(int timeSinceLastFrame){

	if (mDirection == Vector3::ZERO) 
	{
		if (nextLocation()) 
		{
		}
		else if(cdAstar < 0)
		{
			mAnimationState = mEntity->getAnimationState("Walk");
			mAnimationState->setLoop(true);
			mAnimationState->setEnabled(true);
			cdAstar = 3000;
			newAstar();
		}
		else
		{
			mAnimationState = mEntity->getAnimationState("Idle");
			mAnimationState->setLoop(true);
			mAnimationState->setEnabled(true);
			cdAstar -= timeSinceLastFrame;
		}
	}
	else
	{
		Real move = mWalkSpeed * timeSinceLastFrame/1000;
		mDistance -= move;
		cdAstar -= timeSinceLastFrame;
		if (mDistance <= 0.0f)
		{
			mNode->setPosition(mDestination);
			mDirection = Vector3::ZERO;
			// Set animation based on if the robot has another point to walk to. 
			if ( nextLocation())
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
				}
			}
		}
		else
		{
			mNode->translate(mDirection * move);
		}
	}
}
bool DemoApp::nextLocation(){
	if (mWalkList.empty())
		return false;
	mDestination = mWalkList.front();
	mWalkList.pop_front();       
	mDirection = mDestination - mNode->getPosition();
	mDistance = mDirection.normalise();
	return true;
}
void DemoApp::setNotWalkables(){
	for(int i = 0;i <16;i++){
		graphMap[15][14+ i]->setWalkable(false);
	}

	graphMap[16][14]->setWalkable(false);
	graphMap[17][14]->setWalkable(false);
	graphMap[15][12]->setWalkable(false);
	graphMap[17][12]->setWalkable(false);
	graphMap[19][12]->setWalkable(false);
}

//Returns all entities that are of interest for the echo
vector<MovableObject*> DemoApp::getEntities()
{
	vector<MovableObject*> entities;
	vector<SceneNode*> allSceneNodes;

	Ogre::SceneNode::ChildNodeIterator cNI = sceneMgr->getRootSceneNode()->getChildIterator();

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

	return(entities);
}

//Returns the box values for the echo calculations
vector<float> DemoApp::getBoxValues(vector<MovableObject*> entityList)
{

	//Find all entities and send their needed input into the playSoundWithEcho() method
	vector<float> boxValues;

	for(int y = 0; y < entityList.size(); y++)
		boxValues.push_back(entityList[y]->getBoundingBox().volume());

	return(boxValues);
}

//Returns the box positions for the echo calculations
vector<vector<float>> DemoApp::getBoxPositions(vector<MovableObject*> entityList)
{
	vector<vector<float>> boxPositions;

	for(int y = 0; y < entityList.size(); y++)
	{
		vector<float> in;
		in.push_back(entityList[y]->getParentSceneNode()->getPosition().x);
		in.push_back(entityList[y]->getParentSceneNode()->getPosition().y);
		in.push_back(entityList[y]->getParentSceneNode()->getPosition().z);
		boxPositions.push_back(in);
	}

	return(boxPositions);
}
//|||||||||||||||||||||||||||||||||||||||||||||||