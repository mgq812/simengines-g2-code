#include "Crosshair.h"

using namespace Ogre;

Crosshair::Crosshair
(
	SceneManager *sceneMgr,
	Camera *camera,
	int xSize,
	int ySize,
	int distance
):
	cSceneMgr (sceneMgr),
	cCamera (camera),
	crossXSize (xSize),
	crossYSize (ySize),
	crossDistance (distance)
{
	createCross("Crosshair/Cross1");
}

bool Crosshair::frameStarted(const FrameEvent &e) 
{	
	crossNode->setPosition(cCamera->getPosition()+(cCamera->getDirection()*crossDistance));
	crossNode->lookAt(cCamera->getPosition(), 
			Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
	SceneNode* temp = cSceneMgr->getRootSceneNode()->createChildSceneNode("temp");
	temp->setPosition(cCamera->getPosition()+(cCamera->getDirection()*2500));
	temp->setOrientation(crossNode->getOrientation());
	temp->lookAt(cCamera->getPosition(), 
		Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
	Radian diffAngle = temp->getOrientation().zAxis().angleBetween(cCamera->getUp());
	temp->yaw(diffAngle);
	if ((temp->getOrientation().zAxis().angleBetween(cCamera->getUp())) > Radian(0)) {
		crossNode->yaw(-diffAngle);
	} else {
		crossNode->yaw(diffAngle);
	}
	cSceneMgr->getRootSceneNode()->removeAndDestroyChild("temp");
	return true;
}

void Crosshair:: createCross(String materialName)
{
	Plane plane(Vector3(0,-1,0), 0);
	MeshManager::getSingleton().createPlane("crosshair",
       ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane,
       crossXSize,crossYSize,1,1,true,1,1,1,Vector3::UNIT_Z);
	crossEntity = cSceneMgr->createEntity("CrossEntity", "crosshair");
	crossNode = cSceneMgr->getRootSceneNode()->createChildSceneNode("CrossNode");
	crossNode->attachObject(crossEntity);
	crossEntity->setMaterialName(materialName);
	crossEntity->setCastShadows(false);
}
