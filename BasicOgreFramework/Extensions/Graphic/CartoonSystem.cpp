#include "CartoonSystem.h"

using namespace Ogre;

namespace CartoonCaelum {
	
	CartoonSystem::CartoonSystem
	(
		Root *pRoot,
		SceneManager *pSceneMgr,
		Camera *pCamera
	):
		m_pRoot (pRoot),
		m_pSceneMgr (pSceneMgr),
		m_pCamera (pCamera),
		m_pWindCloud(0),
		m_pSun(0),
		m_pMoon(0),
		m_fIsDay(true),
		m_fpTimer(Real(0)),
		m_fpUpdateTime(Real(0.2)),
		m_pRainNode(0),
		m_pSnowNode(0),
		m_pRainPS(0),
		m_pSnowPS(0),
		m_pvWindVector(0)
	{	
		setupResources();
		loadResources();
		makeSky();
		makeSun();
		makeMoon();
	}

	CartoonSystem::~CartoonSystem()
	{
		removeRain();
		removeSnow();
		removeWindCloud();
		if (m_pSun!=0) {
			delete m_pSun;
		}
		if (m_pMoon!=0) {
			delete m_pMoon;
		}
	}

	void CartoonSystem::setupResources()
	{
		String _strSecName, _strTypeName, _strArchName;
		ConfigFile _cf;
        _cf.load("CartoonResources.cfg");
		ConfigFile::SectionIterator _seci = _cf.getSectionIterator();
        while (_seci.hasMoreElements())
        {
			_strSecName = _seci.peekNextKey();
			ConfigFile::SettingsMultiMap *settings = _seci.getNext();
			ConfigFile::SettingsMultiMap::iterator _iter;
			for (_iter=settings->begin(); _iter != settings->end(); ++_iter) {
                _strTypeName = _iter->first;
                _strArchName = _iter->second;
				ResourceGroupManager::getSingleton().addResourceLocation(_strArchName, _strTypeName, _strSecName);
            }
        }
	}

	void CartoonSystem::loadResources()
	{
        ResourceGroupManager::getSingleton().initialiseResourceGroup("Cartoon");
	}

	void CartoonSystem::addRain()
	{
		//if rainPS points to null, create rain particlesystem and scene node.
		if (m_pRainPS==0) {
			m_pRainPS = m_pSceneMgr->createParticleSystem("rainSystem", "Cartoon/RainFall");
			m_pRainNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("rainNode");
			m_pRainNode->attachObject(m_pRainPS);
			//if windWector do not point to null, add wind vector to all currently running particle systems.
			if (m_pvWindVector!=0) {
				addWindVector(*m_pvWindVector);
			}
		}
	}

	void CartoonSystem::addSnow()
	{
		//if snowPS points to null, create snow particlesystem and scene node.
		if (m_pSnowPS==0) {
			m_pSnowPS = m_pSceneMgr->createParticleSystem("snowSystem", "Cartoon/SnowFall");
			m_pSnowNode = m_pSceneMgr->getRootSceneNode()->createChildSceneNode("snowNode");
			m_pSnowNode->attachObject(m_pSnowPS);
			//if windWector do not point to null, add wind vector to all currently running particle systems.
			if (m_pvWindVector!=0) {
				addWindVector(*m_pvWindVector);
			}
		}
	}
	
	bool CartoonSystem::hasRain()
	{
		return m_pRainPS!=0;
	}

	bool CartoonSystem::hasSnow()
	{
		return m_pSnowPS!=0;
	}

	void CartoonSystem::removeRain()
	{
		//if rainPS do not point to null, remove rain particle system and rain scene node
		//from the scene manager, and set their pointers to null.
		if (m_pRainPS!=0) {
			m_pSceneMgr->getRootSceneNode()->removeAndDestroyChild("rainNode");
			m_pSceneMgr->destroyParticleSystem("rainSystem");
			m_pRainPS = 0;
			m_pRainNode = 0;
		}
	}

	void CartoonSystem::removeSnow()
	{	
		//if snowPS do not point to null, remove snow particle system and snow scene node
		//from the scene manager, and set their pointers to null.
		if (m_pSnowPS!=0) {
			m_pSceneMgr->getRootSceneNode()->removeAndDestroyChild("snowNode");
			m_pSceneMgr->destroyParticleSystem("snowSystem");
			m_pSnowPS = 0;
			m_pSnowNode = 0;
		}
	}

	void CartoonSystem::setRainDensity(int nDensity)
	{	
		//if rainPS does not point to null, set emission_rate for its emitters.
		if (m_pRainPS!=0) {
			for (unsigned short _ixus=0; _ixus<m_pRainPS->getNumEmitters(); _ixus++) {
				m_pRainPS->getEmitter(_ixus)->setParameter("emission_rate", StringConverter::toString(nDensity));
			}
		}
	}

	void CartoonSystem::setSnowDensity(int nDensity)
	{
		//if snowPS does not point to null, set emission_rate for its emitters.
		if (m_pSnowPS!=0) {
			for (unsigned short _ixus=0; _ixus<m_pSnowPS->getNumEmitters(); _ixus++) {
				m_pSnowPS->getEmitter(_ixus)->setParameter("emission_rate", StringConverter::toString(nDensity));
			}
		}
	}

	void CartoonSystem::setRainVelocity(int nMin, int nMax)
	{
		//if rainPS does not point to null, set velocity for its emitters.
		if (m_pRainPS!=0) {
			for (unsigned short _ixus=0; _ixus<m_pRainPS->getNumEmitters(); _ixus++) {
				m_pRainPS->getEmitter(_ixus)->setParameter("velocity_min", StringConverter::toString(nMin));
				m_pRainPS->getEmitter(_ixus)->setParameter("velocity_max", StringConverter::toString(nMax));
			}
		}
	}

	void CartoonSystem::setSnowVelocity(int nMin, int nMax)
	{
		//if snowPS does not point to null, set velocity for its emitters.
		if (m_pSnowPS!=0) {
			for (unsigned short _ixus=0; _ixus<m_pSnowPS->getNumEmitters(); _ixus++) {
				m_pSnowPS->getEmitter(_ixus)->setParameter("velocity_min", StringConverter::toString(nMin));
				m_pSnowPS->getEmitter(_ixus)->setParameter("velocity_max", StringConverter::toString(nMax));
			}
		}
	}

	void CartoonSystem::addWindVector(Vector3 vWind) 
	{
		//if rainPS does not point to null, add wind vector to the rain particle system.
		if (m_pRainPS!=0) {
			//if there is already a wind vector in place, remove the old one from the rain particle system.
			if (m_pvWindVector!=0) {
				m_pRainPS->removeAffector(m_pRainPS->getNumAffectors()-1);
			}
			//create new affector, with parameters force_vector and force_application, and add it to the
			//rain particle system.
			m_pRainPS->addAffector("LinearForce");
			m_pRainPS->getAffector(m_pRainPS->getNumAffectors()-1)->setParameter("force_vector", StringConverter::toString(vWind.x)
				+" "+StringConverter::toString(vWind.y)+" "+StringConverter::toString(vWind.z));
			m_pRainPS->getAffector(m_pRainPS->getNumAffectors()-1)->setParameter("force_application", "add");
		}
		//if snowPS does not point to null, add wind vector to the snow particle system.
		if (m_pSnowPS!=0) {
			//if there is already a wind vector in place, remove the old one from the snow particle system.
			if (m_pvWindVector!=0) {
				m_pSnowPS->removeAffector(m_pSnowPS->getNumAffectors()-1);
			}
			//create new affector, with parameters force_vector and force_application, and add it to the
			//snow particle system.
			m_pSnowPS->addAffector("LinearForce");
			m_pSnowPS->getAffector(m_pSnowPS->getNumAffectors()-1)->setParameter("force_vector", StringConverter::toString(vWind.x)
				+" "+StringConverter::toString(vWind.y)+" "+StringConverter::toString(vWind.z));
			m_pSnowPS->getAffector(m_pSnowPS->getNumAffectors()-1)->setParameter("force_application", "add");
		}
		//create a wind cloud and position it in the opposite direction of the wind vector.
		makeWindCloud();
		m_pWindCloud->setPosition(Vector3(-vWind.x, 1, -vWind.z));
		m_pvWindVector = new Vector3(vWind);
	}

	void CartoonSystem::removeWindVector()
	{
		//remove wind cloud and point windVector to null.
		removeWindCloud();
		m_pvWindVector = 0;
	}

	void CartoonSystem::removeWindCloud()
	{
		//if cWindCloud does not point to null, delete the WindCloud object.
		if (m_pWindCloud!=0) {
			delete m_pWindCloud;
		}
		m_pWindCloud = 0;
	}

	Sun* CartoonSystem::getSun()
	{
		return m_pSun;
	}

	Moon* CartoonSystem::getMoon()
	{
		return m_pMoon;
	}

	Real CartoonSystem::getUpdateTime()
	{
		return m_fpUpdateTime;
	}

	void CartoonSystem::setUpdateTime(Real updateTime)
	{
		m_fpUpdateTime = updateTime;
	}

	void CartoonSystem::makeSky()
	{
		//create a skydome
		m_pSceneMgr->setSkyDome(true, "Cartoon/WeirdSky2", 5, 8, 10000);
	}

	void CartoonSystem::makeSun()
	{
		//create a sun, and set its current face.
		m_pSun = new Sun(m_pSceneMgr, m_pCamera, m_pSceneMgr->getRootSceneNode()->createChildSceneNode(),
			512, 512, 6600, Degree(45));
		m_pSun->getFace()->setCurrentMaterial("Cartoon/HappyFace");
	}

	void CartoonSystem::makeMoon()
	{
		//create a moon, and set its current face.
		m_pMoon = new Moon(m_pSceneMgr, m_pCamera, m_pSceneMgr->getRootSceneNode()->createChildSceneNode(),
			512, 512, 6300, Degree(45), FULL);
	}

	void CartoonSystem::makeWindCloud()
	{
		//remove any old wind cloud and create a new one.
		removeWindCloud();
		m_pWindCloud = new CartoonSkyObject(m_pSceneMgr, m_pCamera, m_pSceneMgr->getRootSceneNode()->createChildSceneNode(),
			"Cartoon/Cloud1", "Cartoon/BlowingFace", 2000, 2000, 6000, Real(0.25), Real(0.25));
	}

	bool CartoonSystem::frameStarted(const FrameEvent &e) 
	{	
		updateComponents(e.timeSinceLastFrame);
		return true;
	}

	void CartoonSystem::updateComponents(Real fpTimeSinceLastFrame)
	{
		//update all components that should update each frame.
		updateParticleFacing();
		m_fpTimer += fpTimeSinceLastFrame;
		//update all components that are time based.
		if (m_fpTimer > 0.02) {
			m_fpTimer -= 0.02;
			updateSun();
			updateMoon();
			updateSky();
			updateWindCloud();
		}
	}
	
	void CartoonSystem::updateParticleFacing()
	{
		//if rainNode does not point to null, set its position with respect to the
		//camera and any wind vector.
		if (m_pRainNode!=0) {
			m_pRainNode->setPosition(m_pCamera->getPosition()+(m_pCamera->getDirection()*(40)));
			if (m_pvWindVector!=0) {
				m_pRainNode->translate(-*m_pvWindVector*2);
			}
		}
		//if snowNode does not point to null, set its position with respect to the
		//camera and any wind vector.
		if (m_pSnowNode!=0) {
			m_pSnowNode->setPosition(m_pCamera->getPosition()+(m_pCamera->getDirection()*(40)));
			if (m_pvWindVector!=0) {
				m_pSnowNode->translate(-*m_pvWindVector*2);
			}
		}
	}


	void CartoonSystem::updateSky()
	{
		//if the sun is up set daytime sky, else set nighttime sky.
		if (m_pSun->getNode()->getPosition().y > -(m_pSun->getYSize()/2)) {
			if (!m_fIsDay) {
				m_pSceneMgr->setSkyDome(true, "Cartoon/WeirdSky2", 5, 8, 10000);
				m_fIsDay = true;
			}
		} else {
			if (m_fIsDay) {
				m_pSceneMgr->setSkyDome(true, "Cartoon/NightSky", 5, 8, 10000);
				m_fIsDay = false;
			}
		}
	}

	void CartoonSystem::updateSun()
	{
		//move the sun a certain degree and update the direction of its face.
		m_pSun->moveObject(Radian(Degree(0.1)));
		m_pSun->updateLight();
		m_pSun->directObject();
		if (m_pSun->getNode()->getPosition().y < 1000) {
			//if the sun is close to going down set a sleepy face on it.
			if (m_pSun->getFace()->getCurrentMaterial()=="Cartoon/HappyFace") {
				m_pSun->getFace()->setCurrentMaterial("Cartoon/SleepyFace");
			}
		} else {
			//if the sun has just gone up, set a happy face on it.
			if (m_pSun->getFace()->getCurrentMaterial()== "Cartoon/SleepyFace") {
				m_pSun->getFace()->setCurrentMaterial("Cartoon/HappyFace");
			}
		}
	}

	void CartoonSystem::updateMoon()
	{
		//move the moon a certain degree and update the direction of its face.
		m_pMoon->moveObject(Radian(Degree(0.1)));
		m_pMoon->updateLight();
		m_pMoon->directObject();
	}

	void CartoonSystem::updateWindCloud()
	{
		//if cWindCloud does not point to null, update the direction of its face.
		if (m_pWindCloud!=0) {
			m_pWindCloud->getFace()->directEntity();
		}
	}

}
