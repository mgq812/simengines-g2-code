#include "Cloud.h"

using namespace Ogre;

namespace CartoonCaelum {

	Cloud::Cloud
	(
		SceneManager *sceneMgr,
		Camera *camera,
		int xSize,
		int ySize,
		int distance
	):
		cSceneMgr (sceneMgr),
		cCamera (camera),
		cloudXSize (xSize),
		cloudYSize (ySize),
		cloudDistance (distance)
	{
		createCloud("Cartoon/Sun");
		cloudFace = new Face(cSceneMgr, cCamera, 
			cloudNode->createChildSceneNode("FaceNode" + InternalUtilities::pointerToString(this)),
			InternalUtilities::round(cloudXSize*0.9), 
			InternalUtilities::round(cloudYSize*0.9), 200);
		cloudFace->setFace("Cartoon/BlowingFace");
	}

	void Cloud::setPosition(Vector3 newPos)
	{
		newPos.normalise();
		cloudNode->setPosition(newPos*cloudDistance);
	}

	SceneNode *Cloud::getNode()
	{
		return cloudNode;
	}

	Face *Cloud::getFace()
	{
		return cloudFace;
	}

	void Cloud:: createCloud(String materialName)
	{
		String uniqueSuffix = InternalUtilities::pointerToString(this);
		Plane plane(Vector3(0,-1,0), 0);
		MeshManager::getSingleton().createPlane("cloud"+uniqueSuffix,
           ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
           cloudXSize,cloudYSize,1,1,true,1,1,1,Vector3::UNIT_Z);
		cloudEntity = cSceneMgr->createEntity("CloudEntity"+uniqueSuffix, "cloud"+uniqueSuffix);
		cloudNode = cSceneMgr->getRootSceneNode()->createChildSceneNode("CloudNode"+uniqueSuffix);
		cloudNode->attachObject(cloudEntity);
		cloudEntity->setMaterialName(materialName);
		cloudEntity->setCastShadows(false);
		cloudNode->setPosition(0, cloudDistance, 0);
	}

}
