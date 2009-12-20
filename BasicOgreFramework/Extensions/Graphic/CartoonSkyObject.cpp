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
		m_pFace = new Face(m_pSceneMgr, m_pCamera, 
			m_pMainNode->createChildSceneNode("FaceNode"+m_strUniqueSuffix),
			strFaceMaterialName, InternalUtilities::round(m_nXSize*fpFaceXRatio),
			InternalUtilities::round(m_nYSize*fpFaceYRatio), 200);
	}

	CartoonSkyObject::~CartoonSkyObject()
	{
		
	}

	Face* CartoonSkyObject::getFace()
	{
		return m_pFace;
	}

}
