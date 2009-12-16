#include "Sun.h"

using namespace Ogre;

namespace CartoonCaelum {

	Sun::Sun
	(
		SceneManager *sceneMgr,
		Camera *camera,
		int xSize,
		int ySize,
		int distance,
		Radian pitch
	):
		cSceneMgr (sceneMgr),
		cCamera (camera),
		sunXSize (xSize),
		sunYSize (ySize),
		sunDistance (distance),
		cyclePitch (pitch)
	{
		previousRotation = Radian(0);
		createSun("Cartoon/Sun");
		createMood();
		createLight();
	}

	void Sun::setMood(String materialName) 
	{
		moodEntity->setMaterialName(materialName);
		moodEntity->setVisible(true);
	}

	void Sun::moveSun(Radian degrees)
	{
		Vector3 position = sunNode->getPosition();
		previousRotation += degrees;
		int newX = InternalUtilities::round(sunDistance*(Math::Sin(previousRotation, false)));
		int newY = InternalUtilities::round(sunDistance*(Math::Cos(previousRotation, false)));
		int newZ = InternalUtilities::round(newY*(Math::Sin(cyclePitch, false))); 
		sunNode->setPosition(Vector3(newX,newY,newZ));
		sunNode->lookAt(Vector3(0,0,0), 
			Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
	}

	void Sun::directMood()
	{
		SceneNode* temp = cSceneMgr->getRootSceneNode()->createChildSceneNode("temp");
		temp->setPosition(cCamera->getPosition()+(cCamera->getDirection()*2500));
		temp->setOrientation(moodNode->getOrientation());
		temp->lookAt(cCamera->getPosition(), 
			Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
		Radian diffAngle = temp->getOrientation().zAxis().angleBetween(cCamera->getUp());
		temp->yaw(diffAngle);
		if ((temp->getOrientation().zAxis().angleBetween(cCamera->getUp())) > Radian(0)) {
			moodNode->yaw(-diffAngle);
		} else {
			moodNode->yaw(diffAngle);
		}
		cSceneMgr->getRootSceneNode()->removeAndDestroyChild("temp");
	}

	void Sun:: createSun(String materialName)
	{
		Plane plane(Vector3(0,-1,0), 0);
		MeshManager::getSingleton().createPlane("sun",
           ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
           sunXSize,sunYSize,1,1,true,1,1,1,Vector3::UNIT_Z);
		sunEntity = cSceneMgr->createEntity("SunEntity", "sun");
		sunNode = cSceneMgr->getRootSceneNode()->createChildSceneNode("SunNode");
		sunNode->attachObject(sunEntity);
		sunEntity->setMaterialName(materialName);
		sunEntity->setCastShadows(false);
		sunNode->setPosition(Vector3(0,sunDistance,0));
	}

	void Sun::createMood()
	{
		Plane plane(Vector3(0,-1,0), 200);
		MeshManager::getSingleton().createPlane("mood",
		   ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
		   InternalUtilities::round(sunXSize*0.9),
		   InternalUtilities::round(sunXSize*0.9),
		   1,1,true,1,1,1,Vector3::UNIT_Z);
		moodEntity = cSceneMgr->createEntity("MoodEntity", "mood");
		moodNode = sunNode->createChildSceneNode("MoodNode");
		moodNode->attachObject(moodEntity);
		moodEntity->setCastShadows(false);
		moodEntity->setVisible(false);
	}

	void Sun::createLight()
	{
		sunLight = cSceneMgr->createLight("SunLight");
		sunLight->setType(Light::LT_DIRECTIONAL);
		lightNode = sunNode->createChildSceneNode("LightNode");
		lightNode->attachObject(sunLight);
		sunLight->setPosition(Vector3(0, -1, 0));
		sunLight->setDiffuseColour(0.0, 1.0, 0.1);
		sunLight->setSpecularColour(0.0, 1.0, 0.1);
	}

}
