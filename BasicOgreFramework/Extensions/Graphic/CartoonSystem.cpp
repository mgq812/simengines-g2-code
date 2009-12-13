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
		for (int k=0; k<8; k++) {
			snowNode[k] = cSceneMgr->getRootSceneNode()->createChildSceneNode();
		}
		timer = new Real(0.0);
		setupResources();
		loadResources();
		makeSky();
		makeSun();
		makeWeather();
	}

	bool CartoonSystem::frameStarted(const FrameEvent &e) 
	{	
		updateComponents(e.timeSinceLastFrame);
		return true;
	}

	void CartoonSystem::updateComponents(Real timeSinceLastFrame)
	{
		updateParticleFacing();
		*timer += timeSinceLastFrame;
		if (*timer > 0.02) {
			*timer -= 0.02;
			updateSun();
			updateSky();
		}
	}

	void CartoonSystem::makeSky()
	{
		cSceneMgr->setSkyBox(true, "Cartoon/WeirdSky");
	}

	void CartoonSystem::makeSun()
	{
		cSun = new Sun(cSceneMgr, cCamera, 512, 512, 5500, new Radian(*(new Degree(0))));
		cSun->setMood("Cartoon/SleepyFace");
	}

	void CartoonSystem::makeWeather()
	{
		for (int k=0; k<2; k++) {
			snowNode[k]->attachObject(
				cSceneMgr->createParticleSystem("snowSystem"+k, "Cartoon/SnowFall"));
		}
	}

	void CartoonSystem::updateParticleFacing()
	{
		for (int k=0; k<8; k++) {
			snowNode[k]->setPosition(cCamera->getPosition()+(cCamera->getDirection()*20));
		}
		snowNode[0]->translate(0,100,0);
		snowNode[1]->translate(20,100,0);
		snowNode[2]->translate(40,100,0);
		snowNode[3]->translate(-20,100,0);
		snowNode[4]->translate(-40,100,0);
		snowNode[5]->translate(-60,100,0);
		snowNode[6]->translate(60,100,0);
		snowNode[7]->translate(0,50,0);
	}

	void CartoonSystem::updateSky()
	{

	}

	void CartoonSystem::updateSun()
	{
		cSun->moveSun(new Radian(*(new Degree(0.1))));
		cSun->directMood();
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
			for (i = settings->begin(); i != settings->end(); ++i)
            {
                typeName = i->first;
                archName = i->second;
				ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            }
        }
	}

	void CartoonSystem::loadResources()
	{
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
        ResourceGroupManager::getSingleton().initialiseResourceGroup("Cartoon");
	}

}
