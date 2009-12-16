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
		createCloud("Cartoon/Cloud1");
		cloudFace = new Face(cSceneMgr, cCamera, 
			cloudNode->createChildSceneNode("FaceNode" + InternalUtilities::pointerToString(this)),
			InternalUtilities::round(cloudXSize*0.9), 
			InternalUtilities::round(cloudYSize*0.9), 200);
		cloudFace->setFace("Cartoon/BlowingFace");
	}

	void Cloud::setPosition(Vector3 newPos)
	{
		Vector3 normPos = newPos.normalisedCopy();
		cloudNode->setPosition(normPos*cloudDistance);
		cloudNode->lookAt(Vector3(0,0,0), Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
		Radian diffAngle = cloudNode->getOrientation().zAxis().angleBetween(Vector3::UNIT_Y + normPos);
		cloudNode->yaw(diffAngle);
		if ((cloudNode->getOrientation().zAxis().angleBetween(newPos.perpendicular())) > Radian(0)) {
			cloudNode->yaw(-diffAngle*2);
		} else {
			cloudNode->yaw(diffAngle);
		}
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
