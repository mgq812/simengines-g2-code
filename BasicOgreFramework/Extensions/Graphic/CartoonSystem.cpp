#include "CartoonSystem.h"

using namespace Ogre;

namespace CartoonCaelum {

	CartoonSystem::CartoonSystem
	(
		Root *root,
		SceneManager *sceneMgr,
		Camera *camera
	):
		cRoot (root),
		cSceneMgr (sceneMgr),
		cCamera (camera)
	{	
		cWindCloud = 0;
		windVector = 0;
		snowNode = 0;
		snowPS = 0;
		rainNode = 0;
		rainPS = 0;
		timer = Real(0);
		setupResources();
		loadResources();
		makeSky();
		makeSun();
	}

	void CartoonSystem::configure()
	{

	}

	void CartoonSystem::setupResources()
	{
		String secName, typeName, archName;
		ConfigFile cf;
        cf.load("CartoonResources.cfg");
		ConfigFile::SectionIterator seci = cf.getSectionIterator();
        while (seci.hasMoreElements())
        {
			secName = seci.peekNextKey();
			ConfigFile::SettingsMultiMap *settings = seci.getNext();
			ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i) {
                typeName = i->first;
                archName = i->second;
				ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            }
        }
	}

	void CartoonSystem::loadResources()
	{
        ResourceGroupManager::getSingleton().initialiseResourceGroup("Cartoon");
	}

	void CartoonSystem::addRain()
	{
		if (rainPS==0) {
			rainPS = cSceneMgr->createParticleSystem("rainSystem", "Cartoon/RainFall");
			rainNode = cSceneMgr->getRootSceneNode()->createChildSceneNode("rainNode");
			rainNode->attachObject(rainPS);
			if (windVector!=0) {
				addWindVector(*windVector);
			}
		}
	}

	void CartoonSystem::addSnow()
	{
		if (snowPS==0) {
			snowPS = cSceneMgr->createParticleSystem("snowSystem", "Cartoon/SnowFall");
			snowNode = cSceneMgr->getRootSceneNode()->createChildSceneNode("snowNode");
			snowNode->attachObject(snowPS);
			if (windVector!=0) {
				addWindVector(*windVector);
			}
		}
	}

	bool CartoonSystem::hasRain()
	{
		return rainPS!=0;
	}

	bool CartoonSystem::hasSnow()
	{
		return snowPS!=0;
	}

	void CartoonSystem::removeRain()
	{
		if (rainPS!=0) {
			cSceneMgr->getRootSceneNode()->removeAndDestroyChild("rainNode");
			cSceneMgr->destroyParticleSystem("rainSystem");
			rainPS = 0;
			rainNode = 0;
		}
	}

	void CartoonSystem::removeSnow()
	{
		if (snowPS!=0) {
			cSceneMgr->getRootSceneNode()->removeAndDestroyChild("snowNode");
			cSceneMgr->destroyParticleSystem("snowSystem");
			snowPS = 0;
			snowNode = 0;
		}
	}

	void CartoonSystem::setRainDensity(int density)
	{
		if (rainPS!=0) {
			for (unsigned short index = 0; index<rainPS->getNumEmitters(); index++) {
				rainPS->getEmitter(index)->setParameter("emission_rate", StringConverter::toString(density));
			}
		}
	}

	void CartoonSystem::setSnowDensity(int density)
	{
		if (snowPS!=0) {
			for (unsigned short index = 0; index<snowPS->getNumEmitters(); index++) {
				snowPS->getEmitter(index)->setParameter("emission_rate", StringConverter::toString(density));
			}
		}
	}

	void CartoonSystem::setRainVelocity(int min, int max)
	{
		if (rainPS!=0) {
			for (unsigned short index = 0; index<rainPS->getNumEmitters(); index++) {
				rainPS->getEmitter(index)->setParameter("velocity_min", StringConverter::toString(min));
				rainPS->getEmitter(index)->setParameter("velocity_max", StringConverter::toString(max));
			}
		}
	}

	void CartoonSystem::setSnowVelocity(int min, int max)
	{
		if (snowPS!=0) {
			for (unsigned short index = 0; index<snowPS->getNumEmitters(); index++) {
				snowPS->getEmitter(index)->setParameter("velocity_min", StringConverter::toString(min));
				snowPS->getEmitter(index)->setParameter("velocity_max", StringConverter::toString(max));
			}
		}
	}

	void CartoonSystem::addWindVector(Vector3 wind) 
	{
		if (snowPS!=0) {
			if (windVector!=0) {
				snowPS->removeAffector(snowPS->getNumAffectors()-1);
			}
			snowPS->addAffector("LinearForce");
			snowPS->getAffector(snowPS->getNumAffectors()-1)->setParameter("force_vector", StringConverter::toString(wind.x)
				+" "+StringConverter::toString(wind.y)+" "+StringConverter::toString(wind.z));
			snowPS->getAffector(snowPS->getNumAffectors()-1)->setParameter("force_application", "add");
		}
		if (rainPS!=0) {
			if (windVector!=0) {
				rainPS->removeAffector(rainPS->getNumAffectors()-1);
			}
			rainPS->addAffector("LinearForce");
			rainPS->getAffector(rainPS->getNumAffectors()-1)->setParameter("force_vector", StringConverter::toString(wind.x)
				+" "+StringConverter::toString(wind.y)+" "+StringConverter::toString(wind.z));
			rainPS->getAffector(rainPS->getNumAffectors()-1)->setParameter("force_application", "add");
		}
		makeWindCloud();
		cWindCloud->setPosition(Vector3(-wind.x, 1, -wind.z));
		windVector = new Vector3(wind);
	}

	void CartoonSystem::removeWindVector()
	{
		cWindCloud = 0;
		windVector = 0;
	}

	Sun* CartoonSystem::getSun()
	{
		return cSun;
	}

	bool CartoonSystem::frameStarted(const FrameEvent &e) 
	{	
		updateComponents(e.timeSinceLastFrame);
		return true;
	}

	void CartoonSystem::updateComponents(Real timeSinceLastFrame)
	{
		updateParticleFacing();
		timer += timeSinceLastFrame;
		if (timer > 0.02) {
			timer -= 0.02;
			updateSun();
			updateSky();
			updateWindCloud();
		}
	}
	
	void CartoonSystem::updateParticleFacing()
	{
		if (snowNode!=0) {
			snowNode->setPosition(cCamera->getPosition()+(cCamera->getDirection()*(40)));
			if (windVector!=0) {
				snowNode->translate(-*windVector*2);
			}
		}
		if (rainNode!=0) {
			rainNode->setPosition(cCamera->getPosition()+(cCamera->getDirection()*(40)));
			if (windVector!=0) {
				rainNode->translate(-*windVector*2);
			}
		}
	}

	void CartoonSystem::makeSky()
	{
		cSceneMgr->setSkyDome(true, "Cartoon/WeirdSky2", 5, 8, 10000);
	}

	void CartoonSystem::makeSun()
	{
		cSun = new Sun(cSceneMgr, cCamera, 512, 512, 5600, Degree(0));
		cSun->getFace()->setFace("Cartoon/HappyFace");
	}

	void CartoonSystem::makeWindCloud()
	{
		if (cWindCloud!=0) {
			delete cWindCloud;
			/*for (int k=0; k<sizeof(windNode); k++) {
				cSceneMgr->destroyParticleSystem("windSystem"+StringConverter::toString(k));
			}*/
		}
		cWindCloud = new Cloud(cSceneMgr, cCamera, 2000, 2000, 5500);
		cWindCloud->getFace()->setFace("Cartoon/BlowingFace");
		/*for (int k=0; k<4; k++) {
			//windNode[k] = cWindCloud->getFace()->getNode()->createChildSceneNode();
			windNode[k] = cSceneMgr->getRootSceneNode()->createChildSceneNode();
			windNode[k]->setOrientation(cWindCloud->getFace()->getNode()->getOrientation());
			windPS[k] = cSceneMgr->createParticleSystem("windSystem"+
				StringConverter::toString(k), "Cartoon/WindFlow");
			windNode[k]->attachObject(windPS[k]);
			windNode[0]->translate(Vector3(0,0,-20));
		}
		windNode[0]->translate(Vector3(0,0,5));
		windNode[1]->translate(Vector3(5,0,0));
		windNode[2]->translate(Vector3(-5,0,0));
		windNode[3]->translate(Vector3(0,0,-5));
		windNode[2]->yaw(-Degree(45));
		windNode[1]->yaw(Degree(90));
		windNode[3]->yaw(Degree(135));
		//windNode[4]->translate(Vector3(20,0,0));*/
	}

	void CartoonSystem::updateSky()
	{
		if (cSun->isLightChanged()) {
			if (cSun->getNode()->getPosition().y > 0) {
				cSceneMgr->setSkyDome(true, "Cartoon/WeirdSky2", 5, 8, 10000);
			} else {
				cSceneMgr->setSkyDome(true, "Cartoon/NightSky", 5, 8, 10000);
			}
			cSun->flipLightChanged();
		}
	}

	void CartoonSystem::updateSun()
	{
		cSun->moveSun(Radian(Degree(0.1)));
		cSun->getFace()->directFace();
		if (cSun->getNode()->getPosition().y < 1000) {
			if (cSun->getFace()->getCurrentFace()== "Cartoon/HappyFace") {
				cSun->getFace()->setFace("Cartoon/SleepyFace");
			}
		} else {
			if (cSun->getFace()->getCurrentFace()== "Cartoon/SleepyFace") {
				cSun->getFace()->setFace("Cartoon/HappyFace");
			}
		}
	}

	void CartoonSystem::updateWindCloud()
	{
		if (cWindCloud!=0) {
			cWindCloud->getFace()->directFace();
		}
	}

}
