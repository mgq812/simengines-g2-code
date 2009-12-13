//|||||||||||||||||||||||||||||||||||||||||||||||

#include "DemoApp.hpp"

#include <OgreLight.h>
#include <OgreWindowEventUtilities.h>
#include "math.h"
#include "CartoonSystem.h"
//|||||||||||||||||||||||||||||||||||||||||||||||



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
		
	NxOgre::World* mWorld;
	NxOgre::Scene* mScene;
	OGRE3DRenderSystem* mRenderSystem;
	//mWorld = NxOgre::World::createWorld();
	/*
	NxOgre::SceneDescription sceneDesc;
	sceneDesc.mGravity = NxOgre::Vec3(0, -9.8f, 0);
	sceneDesc.mName = "BloodyMessTutorial2";

	mScene = mWorld->createScene(sceneDesc);*/

	//The sky system
	CartoonCaelum::CartoonSystem* cartoon = 
		new CartoonCaelum::CartoonSystem(OgreFramework::getSingletonPtr()->m_pRoot, 
		OgreFramework::getSingletonPtr()->m_pSceneMgr, OgreFramework::getSingletonPtr()->m_pCamera);
	cartoon->setWindVector(Ogre::Vector3(50,0,0));
	OgreFramework::getSingletonPtr()->m_pRoot->addFrameListener(cartoon);


	vector<vector<AstarNode*>> graphMap = Astar::GenerateGraphMap(1);
	//OgreFramework::getSingletonPtr()->m_pSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
	//OgreFramework::getSingletonPtr()->m_pSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
	//OgreFramework::getSingletonPtr()->m_pSceneMgr->setWorldGeometry("terrain.cfg");

	//Create light
	OgreFramework::getSingletonPtr()->m_pSceneMgr->createLight("Light")->setPosition(75,75,75);

	//Creating the ground	
	//m_pCubeEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("2", "models\\ninja.mesh");
	//m_pCubeNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode3");
	//m_pCubeNode->attachObject(m_pCubeEntity);
	//m_pCubeNode->setScale(Vector3(100.0f, 1.0f, 100.0f));
	
	//Creating the character
	//m_pCubeEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("0", "models\\ogrehead.mesh");
	m_pCubeEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("0", "ogrehead.mesh");
	m_pCubeNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode", Vector3(100.0f, 0.0f, 200));
	m_pCubeNode->attachObject(m_pCubeEntity);
	
	//Creating a fish
	m_pCubeEntity = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("1", "fish.mesh");
	m_pCubeNode = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode("CubeNode2", Vector3(0.0f, 0.0f, 500));
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
	
	double timeSinceLastFrame = 0;
	double startTime = 0;

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

			play.setListenerPosition(OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->getAttachedObject("0")->getParentSceneNode()->getPosition().x, OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->getAttachedObject("0")->getParentSceneNode()->getPosition().y, OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->getAttachedObject("0")->getParentSceneNode()->getPosition().z);

			//Movements
			if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_W))
			{
				OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->translate(0, 0, -0.1*timeSinceLastFrame, Node::TS_LOCAL);
			}
			if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_A))
			{
				OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->translate(-0.1*timeSinceLastFrame, 0, 0, Node::TS_LOCAL);
			}
			if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_S))
			{
				OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->translate(0, 0, 0.1*timeSinceLastFrame, Node::TS_LOCAL);
			}
			if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_D))
			{
				OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->translate(0.1*timeSinceLastFrame, 0, 0, Node::TS_LOCAL);
			}
			if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_Z))
			{
				OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->translate(0, 0.1*timeSinceLastFrame, 0, Node::TS_LOCAL);
			}
			if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_X))
			{
				OgreFramework::getSingletonPtr()->m_pSceneMgr->getSceneNode("CubeNode")->translate(0, -0.1*timeSinceLastFrame, 0, Node::TS_LOCAL);
			}
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
