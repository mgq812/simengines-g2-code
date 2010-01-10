#include "Moon.h"

using namespace Ogre;

namespace CartoonCaelum {

	Moon::Moon
	(
		SceneManager *pSceneMgr,
		Camera *pCamera,
		SceneNode *pNode,
		int nXSize,
		int nYSize,
		int nDistance,
		Radian pitch,
		MoonPhase eCurPhase
	):
		CartoonSkyObject(pSceneMgr, pCamera, pNode, "", "", nXSize, 
			nYSize, nDistance, Real(1.3), Real(1.3)),
		RotatingSkyObject(pNode, nDistance, pitch)
	{
		setDefaultMaterials();
		setPhase(eCurPhase);
		createLight();
		//rotating 180 degrees made the face go upside down, so this is a hack.
		moveObject(Degree(90));
		moveObject(Degree(90));
	}

	Moon::~Moon()
	{
		m_pSceneMgr->destroyLight(m_pMoonLight);
	}

	void Moon::updateLight()
	{
		//if the sun's new y-value is higher than half the y-size of the sun plane mesh,
		//and the light is invisible, make it visible, otherwise make it invisible.
		m_pMoonLight->setVisible(m_pMainNode->getPosition().y > -m_nYSize/2);
	}

	String Moon::getPhaseMaterial(MoonPhase phase)
	{
		return m_straPhaseMaterials[phase];
	}

	void Moon::setPhaseMaterial(MoonPhase phase, String material)
	{
		m_straPhaseMaterials[phase] = material;
		//if the moon is in the phase which material was changed, update the material.
		if (m_eCurPhase == phase) {
			setCurrentMaterial(material);
		}
	}

	String Moon::getFaceMaterial(MoonPhase phase)
	{
		return m_straFaceMaterials[phase];
	}

	void Moon::setFaceMaterial(MoonPhase phase, String material)
	{
		m_straFaceMaterials[phase] = material;
		//if the moon is in the phase which material was changed, update the material.
		if (m_eCurPhase == phase) {
			getFace()->setCurrentMaterial(material);
		}
	}

	void Moon::setPhase(MoonPhase phase)
	{
		m_eCurPhase = phase;
		setCurrentMaterial(getPhaseMaterial(phase));
		getFace()->setCurrentMaterial(getFaceMaterial(phase));
	}


	Light* Moon::getLight()
	{
		return m_pMoonLight;
	}

	void Moon::createLight()
	{
		//create light object and scene node, and set colour values.
		m_pMoonLight = m_pSceneMgr->createLight("SunLight"+m_strUniqueSuffix);
		m_pMoonLight->setType(Light::LT_POINT);
		m_pLightNode = m_pMainNode->createChildSceneNode("LightNode"+m_strUniqueSuffix);
		m_pLightNode->attachObject(m_pMoonLight);
		m_pMoonLight->setPosition(Vector3(0, -1, 0));
		m_pMoonLight->setDiffuseColour(1.0, 1.0, 1.0);
		m_pMoonLight->setSpecularColour(1.0, 1.0, 1.0);
		m_pMoonLight->setCastShadows(true);
	}

	void Moon::setDefaultMaterials()
	{
		m_straPhaseMaterials[0] = "Cartoon/Moon_Full";
		m_straPhaseMaterials[1] = "Cartoon/Moon_WaxG";
		m_straPhaseMaterials[2] = "Cartoon/Moon_FirstQ";
		m_straPhaseMaterials[3] = "Cartoon/Moon_WaxC";
		m_straPhaseMaterials[4] = "";
		m_straPhaseMaterials[5] = "Cartoon/Moon_WanC";
		m_straPhaseMaterials[6] = "Cartoon/Moon_ThirdQ";
		m_straPhaseMaterials[7] = "Cartoon/Moon_WanG";

		m_straFaceMaterials[0] = "Cartoon/Moon_Full_Face";
		m_straFaceMaterials[1] = "Cartoon/Moon_WaxG_Face";
		m_straFaceMaterials[2] = "Cartoon/Moon_FirstQ_Face";
		m_straFaceMaterials[3] = "Cartoon/Moon_WaxC_Face";
		m_straFaceMaterials[4] = "";
		m_straFaceMaterials[5] = "Cartoon/Moon_WanC_Face";
		m_straFaceMaterials[6] = "Cartoon/Moon_ThirdQ_Face";
		m_straFaceMaterials[7] = "Cartoon/Moon_WanG_Face";
	}

}
