#include "SkyObject.h"

using namespace Ogre;

namespace CartoonCaelum {

	SkyObject::SkyObject
	(
		SceneManager *pSceneMgr,
		Camera *pCamera,
		SceneNode *pNode,
		String strMaterialName,
		int nXSize,
		int nYSize,
		int nDistance
	):
		m_pSceneMgr (pSceneMgr),
		m_pCamera (pCamera),
		m_pMainNode (pNode),
		m_nXSize (nXSize),
		m_nYSize (nYSize),
		m_nDistance (nDistance)
	{
		m_strUniqueSuffix = InternalUtilities::pointerToString(this);
		createBody();
		setCurrentMaterial(strMaterialName);
	}

	SkyObject::~SkyObject()
	{
		//remove entity and scene nodes from the scene manager.
		m_pSceneMgr->destroyEntity(m_pMainEntity);
		m_pMainNode->removeAndDestroyAllChildren();
		m_pMainNode->getParentSceneNode()->removeAndDestroyChild(m_pMainNode->getName());
	}

	void SkyObject::setPosition(Vector3 vNewPos)
	{
		//make a vector of magnitude one, and use it to calculate new position.
		Vector3 _vNormPos = vNewPos.normalisedCopy();
		//position the object using the vector, but with zero y-value, and then
		//rotate it so that it's up vector corresponds to the world y-axis.
		m_pMainNode->setPosition(_vNormPos.x*m_nDistance, 0, _vNormPos.z*m_nDistance);
		m_pMainNode->lookAt(Vector3(0,0,0), Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
		Radian _diffAngle = m_pEntityNode->getOrientation().zAxis().angleBetween(Vector3::UNIT_Y);
		m_pEntityNode->yaw(-_diffAngle);
		//check if rotation was done in the correct direction; if not - do the correct rotation.
		if ((m_pEntityNode->getOrientation().zAxis().angleBetween(Vector3::UNIT_Y)) > Radian(0)) {
			m_pEntityNode->yaw(_diffAngle*2);
		}
		//set the position according to distance to origin and the aquired vector.
		m_pMainNode->setPosition(_vNormPos*m_nDistance);
		m_pMainNode->lookAt(Vector3(0,0,0), Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
	}

	SceneNode *SkyObject::getNode()
	{
		return m_pMainNode;
	}

	String SkyObject::getCurrentMaterial()
	{
		return m_strMaterialName;
	}

	void SkyObject::setCurrentMaterial(String strMaterialName) 
	{
		m_strMaterialName = strMaterialName;
		//if material name is "", make object invisible.
		if (m_strMaterialName!="") {
			m_pMainEntity->setMaterialName(m_strMaterialName);
			m_pMainEntity->setVisible(true);
		} else {
			m_pMainEntity->setVisible(false);
		}
	}

	int SkyObject::getDistance()
	{
		return m_nDistance;
	}

	int SkyObject::getXSize()
	{
		return m_nXSize;
	}

	int SkyObject::getYSize()
	{
		return m_nYSize;
	}

	void SkyObject::directEntity()
	{
		directNode(m_pEntityNode);
	}

	void SkyObject::directObject()
	{
		directNode(m_pMainNode);
	}

	void SkyObject::directNode(SceneNode *node)
	{
		SceneNode* _pTemp = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("temp");
		_pTemp->setPosition(m_pCamera->getPosition()+(m_pCamera->getDirection()*2500));
		_pTemp->setOrientation(node->getOrientation());
		_pTemp->lookAt(m_pCamera->getPosition(), 
			Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
		Radian _diffAngle = _pTemp->getOrientation().zAxis().angleBetween(m_pCamera->getUp());
		_pTemp->yaw(_diffAngle);
		if ((_pTemp->getOrientation().zAxis().angleBetween(m_pCamera->getUp())) > Radian(0)) {
			node->yaw(-_diffAngle);
		} else {
			node->yaw(_diffAngle);
		}
		m_pSceneMgr->getRootSceneNode()->removeAndDestroyChild("temp");
	}

	void SkyObject::createBody()
	{
		//create the plane mesh.
		Plane _plane(Vector3::NEGATIVE_UNIT_Y, 0);
		MeshManager::getSingleton().createPlane("skyObject"+m_strUniqueSuffix,
           ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, _plane,
           m_nXSize, m_nYSize, 1, 1, true, 1, 1, 1, Vector3::UNIT_Z);
		//add plane mesh to entity, and attach entity to scene node.
		m_pMainEntity = m_pSceneMgr->createEntity("SkyEntity"+m_strUniqueSuffix, "skyObject"+m_strUniqueSuffix);
		m_pEntityNode = m_pMainNode->createChildSceneNode("EntityNode"+m_strUniqueSuffix);
		m_pEntityNode->attachObject(m_pMainEntity);
		m_pMainEntity->setCastShadows(false);
	}
}
