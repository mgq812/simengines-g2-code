#include "CartoonSkyObject.h"

using namespace Ogre;

namespace CartoonCaelum {

	CartoonSkyObject::CartoonSkyObject
	(
		SceneManager *pSceneMgr,
		Camera *pCamera,
		SceneNode *pNode,
		String strMaterialName,
		String strFaceMaterialName,
		int nXSize,
		int nYSize,
		int nDistance,
		Real fpFaceXRatio,
		Real fpFaceYRatio
	):
		SkyObject(pSceneMgr, pCamera, pNode, strMaterialName, 
			nXSize, nYSize, nDistance)
	{
		//set the position of this object.
		m_pMainNode->setPosition(0, m_nDistance, 0);
		m_pFace = new SkyObject(m_pSceneMgr, m_pCamera, 
			m_pMainNode, strFaceMaterialName, InternalUtilities::round(m_nXSize*fpFaceXRatio),
			InternalUtilities::round(m_nYSize*fpFaceYRatio), 200);
		m_pFace->getNode()->setPosition(0, -m_nDistance, 0);
	}

	CartoonSkyObject::~CartoonSkyObject()
	{

	}

	SkyObject* CartoonSkyObject::getFace()
	{
		return m_pFace;
	}

}
