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
		uniqueSuffix = InternalUtilities::pointerToString(this);
		mainNode = cSceneMgr->getRootSceneNode()->createChildSceneNode("MainNode"+uniqueSuffix);
		createCloud("Cartoon/Cloud1");
		cloudFace = new Face(cSceneMgr, cCamera, 
			mainNode->createChildSceneNode("FaceNode"+uniqueSuffix),
			InternalUtilities::round(cloudXSize*0.25), 
			InternalUtilities::round(cloudYSize*0.25), 200);
		cloudFace->setFace("Cartoon/BlowingFace");
	}

	Cloud::~Cloud()
	{
		cSceneMgr->destroyEntity(cloudEntity);
		mainNode->removeAndDestroyAllChildren();
		mainNode->getParentSceneNode()->removeAndDestroyChild(mainNode->getName());
	}

	void Cloud::setPosition(Vector3 newPos)
	{
		Vector3 normPos = newPos.normalisedCopy();
		mainNode->setPosition(normPos.x*cloudDistance, 0, normPos.z*cloudDistance);
		mainNode->lookAt(Vector3(0,0,0), Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
		Radian diffAngle = cloudNode->getOrientation().zAxis().angleBetween(Vector3::UNIT_Y);
		cloudNode->yaw(-diffAngle);
		if ((cloudNode->getOrientation().zAxis().angleBetween(Vector3::UNIT_Y)) > Radian(0)) {
			cloudNode->yaw(diffAngle*2);
		}
		mainNode->setPosition(normPos*cloudDistance);
		mainNode->lookAt(Vector3(0,0,0), Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
	}

	SceneNode *Cloud::getNode()
	{
		return cloudNode;
	}

	Face* Cloud::getFace()
	{
		return cloudFace;
	}

	void Cloud:: createCloud(String materialName)
	{
		Plane plane(Vector3(0,-1,0), 0);
		MeshManager::getSingleton().createPlane("cloud"+uniqueSuffix,
           ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
           cloudXSize,cloudYSize,1,1,true,1,1,1,Vector3::UNIT_Z);
		cloudEntity = cSceneMgr->createEntity("CloudEntity"+uniqueSuffix, "cloud"+uniqueSuffix);
		cloudNode = mainNode->createChildSceneNode("CloudNode"+uniqueSuffix);
		cloudNode->attachObject(cloudEntity);
		cloudEntity->setMaterialName(materialName);
		cloudEntity->setCastShadows(false);
		mainNode->setPosition(0, cloudDistance, 0);
	}

}
