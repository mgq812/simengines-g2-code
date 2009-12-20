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
		CartoonSkyObject(pSceneMgr, pCamera, pNode, "Cartoon/Sun", "Cartoon/HappyFace", nXSize, 
			nYSize, nDistance, Real(0.9), Real(0.9)),
		m_previousRotation(Radian(0)),
		m_cyclePitch (pitch),
		m_fLightChanged(false)
	{
		createLight();
	}

	Sun::~Sun()
	{
		m_pSceneMgr->destroyLight(m_pSunLight);
	}

	void Sun::moveSun(Radian degrees)
	{
		//add degree to the previously accumulated rotation, and calculate new position of sun.
		m_previousRotation += degrees;
		int _newX = InternalUtilities::round(m_nDistance*(Math::Sin(m_previousRotation, false)));
		int _newY = InternalUtilities::round(m_nDistance*(Math::Cos(m_previousRotation, false)));
		int _newZ = InternalUtilities::round(_newY*(Math::Sin(m_cyclePitch, false))); 
		m_pMainNode->setPosition(Vector3(_newX, _newY, _newZ));
		m_pMainNode->lookAt(Vector3(0,0,0), 
			Node::TransformSpace::TS_WORLD, Vector3::NEGATIVE_UNIT_Y);
		if (m_pMainNode->getPosition().y > -m_nYSize/2 && !m_pSunLight->getVisible()) {
			//if the sun's new y-value is higher than half the y-size of the sun plane mesh,
			//and the light is invisible, make it visible and set mLightChanged flag to true.
			m_pSunLight->setVisible(true);
			flipLightChanged();
		} else if (m_pMainNode->getPosition().y < -m_nYSize/2 && m_pSunLight->getVisible()) {
			//if the sun's new y-value is lower than half the y-size of the sun plane mesh,
			//and the light is visible, make it invisible and set mLightChanged flag to true.
			m_pSunLight->setVisible(false);
			flipLightChanged();
		}
	}

	Light* Sun::getLight()
	{
		return m_pSunLight;
	}

	bool Sun::isLightChanged()
	{
		return m_fLightChanged;
	}

	void Sun::flipLightChanged()
	{
		//flip the value of the mLightChanged flag.
		m_fLightChanged = !m_fLightChanged;
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
