#include "Sun.h"

using namespace Ogre;

namespace CartoonCaelum {

	Sun::Sun
	(
		SceneManager *pSceneMgr,
		Camera *pCamera,
		SceneNode *pNode,
		int nXSize,
		int nYSize,
		int nDistance,
		Radian pitch
	):
		CartoonSkyObject(pSceneMgr, pCamera, pNode, "Cartoon/Sun", "", nXSize, 
			nYSize, nDistance, Real(0.9), Real(0.9)),
		RotatingSkyObject(pNode, nDistance, pitch)
	{
		createLight();
	}

	Sun::~Sun()
	{
		m_pSceneMgr->destroyLight(m_pSunLight);
	}

	void Sun::updateLight()
	{
		//if the sun's new y-value is higher than half the y-size of the sun plane mesh,
		//and the light is invisible, make it visible, otherwise make it invisible.
		m_pSunLight->setVisible(m_pMainNode->getPosition().y > -m_nYSize/2);
	}

	Light* Sun::getLight()
	{
		return m_pSunLight;
	}

	void Sun::createLight()
	{
		//create light object and scene node, and set colour values.
		m_pSunLight = m_pSceneMgr->createLight("SunLight"+m_strUniqueSuffix);
		m_pSunLight->setType(Light::LT_POINT);
		m_pLightNode = m_pMainNode->createChildSceneNode("LightNode"+m_strUniqueSuffix);
		m_pLightNode->attachObject(m_pSunLight);
		m_pSunLight->setPosition(Vector3(0, -1, 0));
		m_pSunLight->setDiffuseColour(1.0, 1.0, 1.0);
		m_pSunLight->setSpecularColour(1.0, 1.0, 1.0);
		m_pSunLight->setCastShadows(true);
	}

}
