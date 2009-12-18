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
		uniqueSuffix = InternalUtilities::pointerToString(this);
		lightChanged = false;
		mainNode = cSceneMgr->getRootSceneNode()->createChildSceneNode("MainNode"+uniqueSuffix);
		previousRotation = Radian(0);
		createSun("Cartoon/Sun");
		createLight();
		sunFace = new Face(cSceneMgr, cCamera, 
			mainNode->createChildSceneNode("FaceNode"+uniqueSuffix),
			InternalUtilities::round(sunXSize*0.9), 
			InternalUtilities::round(sunYSize*0.9), 200);
	}

	Sun::~Sun()
	{
		cSceneMgr->destroyEntity(sunEntity);
		cSceneMgr->destroyLight(sunLight);
		mainNode->removeAndDestroyAllChildren();
		mainNode->getParentSceneNode()->removeAndDestroyChild(mainNode->getName());
	}

	void Sun::moveSun(Radian degrees)
	{
		Vector3 position = mainNode->getPosition();
		previousRotation += degrees;
		int newX = InternalUtilities::round(sunDistance*(Math::Sin(previousRotation, false)));
		int newY = InternalUtilities::round(sunDistance*(Math::Cos(previousRotation, false)));
		int newZ = InternalUtilities::round(newY*(Math::Sin(cyclePitch, false))); 
		mainNode->setPosition(Vector3(newX,newY,newZ));
		mainNode->lookAt(Vector3(0,0,0), 
			Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
		if (mainNode->getPosition().y > 0 && !sunLight->getVisible()) {
			sunLight->setVisible(true);
			flipLightChanged();
		} else if (mainNode->getPosition().y < 0 && sunLight->getVisible()) {
			sunLight->setVisible(false);
			flipLightChanged();
		}
		sunLight->setVisible((mainNode->getPosition().y > 0));
	}

	SceneNode *Sun::getNode()
	{
		return mainNode;
	}

	Face* Sun::getFace()
	{
		return sunFace;
	}

	Light* Sun::getLight()
	{
		return sunLight;
	}

	bool Sun::isLightChanged()
	{
		return lightChanged;
	}

	void Sun::flipLightChanged()
	{
		lightChanged = !lightChanged;
	}

	void Sun:: createSun(String materialName)
	{
		Plane plane(Vector3(0,-1,0), 0);
		MeshManager::getSingleton().createPlane("sun"+uniqueSuffix,
           ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
           sunXSize,sunYSize,1,1,true,1,1,1,Vector3::UNIT_Z);
		sunEntity = cSceneMgr->createEntity("SunEntity"+uniqueSuffix, "sun"+uniqueSuffix);
		sunNode = mainNode->createChildSceneNode("SunNode"+uniqueSuffix);
		sunNode->attachObject(sunEntity);
		sunEntity->setMaterialName(materialName);
		sunEntity->setCastShadows(false);
		mainNode->setPosition(Vector3(0,sunDistance,0));
	}

	void Sun::createLight()
	{
		sunLight = cSceneMgr->createLight("SunLight"+uniqueSuffix);
		sunLight->setType(Light::LT_POINT);
		lightNode = sunNode->createChildSceneNode("LightNode"+uniqueSuffix);
		lightNode->attachObject(sunLight);
		sunLight->setPosition(Vector3(0, -1, 0));
		sunLight->setDiffuseColour(1.0, 1.0, 1.0);
		sunLight->setSpecularColour(1.0, 1.0, 1.0);
		sunLight->setCastShadows(true);
	}

}
