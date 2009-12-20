#include "Face.h"

using namespace Ogre;

namespace CartoonCaelum {

	Face::Face
	(
		SceneManager *pSceneMgr,
		Camera *pCamera,
		SceneNode *pNode,
		String strMaterialName,
		int nXSize,
		int nYSize,
		int nDistance
	):
		SkyObject(pSceneMgr, pCamera, pNode, strMaterialName, 
			nXSize, nYSize, nDistance)
	{
		//position the face object away from the origin.
		m_pMainNode->setPosition(0, -m_nDistance, 0);
	}

	Face::~Face()
	{
		
	}

	void Face::directFace()
	{
		SceneNode* _pTemp = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("temp");
		_pTemp->setPosition(m_pCamera->getPosition()+(m_pCamera->getDirection()*2500));
		_pTemp->setOrientation(m_pMainNode->getOrientation());
		_pTemp->lookAt(m_pCamera->getPosition(), 
			Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
		Radian _diffAngle = _pTemp->getOrientation().zAxis().angleBetween(m_pCamera->getUp());
		_pTemp->yaw(_diffAngle);
		if ((_pTemp->getOrientation().zAxis().angleBetween(m_pCamera->getUp())) > Radian(0)) {
			m_pMainNode->yaw(-_diffAngle);
		} else {
			m_pMainNode->yaw(_diffAngle);
		}
		m_pSceneMgr->getRootSceneNode()->removeAndDestroyChild("temp");
	}

}
