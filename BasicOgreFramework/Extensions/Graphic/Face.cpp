#include "Face.h"

using namespace Ogre;

namespace CartoonCaelum {

	Face::Face
	(
		SceneManager *sceneMgr,
		Camera *camera,
		SceneNode *node,
		int xSize,
		int ySize,
		int distance
	):
		cSceneMgr (sceneMgr),
		cCamera (camera),
		faceNode (node),
		faceXSize (xSize),
		faceYSize (ySize),
		faceDistance (distance)
	{
		createFace();
	}

	void Face::setFace(String materialName) 
	{
		faceEntity->setMaterialName(materialName);
		faceEntity->setVisible(true);
	}

	void Face::directFace()
	{
		SceneNode* temp = cSceneMgr->getRootSceneNode()->createChildSceneNode("temp");
		temp->setPosition(cCamera->getPosition()+(cCamera->getDirection()*2500));
		temp->setOrientation(faceNode->getOrientation());
		temp->lookAt(cCamera->getPosition(), 
			Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
		Radian diffAngle = temp->getOrientation().zAxis().angleBetween(cCamera->getUp());
		temp->yaw(diffAngle);
		if ((temp->getOrientation().zAxis().angleBetween(cCamera->getUp())) > Radian(0)) {
			faceNode->yaw(-diffAngle);
		} else {
			faceNode->yaw(diffAngle);
		}
		cSceneMgr->getRootSceneNode()->removeAndDestroyChild("temp");
	}

	void Face::createFace()
	{
		String uniqueSuffix = InternalUtilities::pointerToString(this);
		Plane plane(Vector3(0,-1,0), faceDistance);
		MeshManager::getSingleton().createPlane("face"+uniqueSuffix,
		   ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane, faceXSize,faceYSize,
		   1,1,true,1,1,1,Vector3::UNIT_Z);
		faceEntity = cSceneMgr->createEntity("FaceEntity"+uniqueSuffix, "face"+uniqueSuffix);
		faceNode->attachObject(faceEntity);
		faceEntity->setCastShadows(false);
		faceEntity->setVisible(false);
	}

}
