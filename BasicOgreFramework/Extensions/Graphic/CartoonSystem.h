#ifndef __CartoonSystem_h_
#define __CartoonSystem_h_

#include <ogre.h>

#include "Cloud.h"
#include "Sun.h"

namespace CartoonCaelum {

	class CartoonSystem: public Ogre::FrameListener 
	{

	public:
		CartoonSystem(
			Ogre::Root *root, 
			Ogre::SceneManager *sceneMgr,
			Ogre::Camera *camera);

		void configure();

		void setupResources();

		void loadResources();

		void addRain();

		void addSnow();

		bool hasRain();

		bool hasSnow();

		void removeRain();

		void removeSnow();

		void setRainDensity(int emissionRate);

		void setSnowDensity(int emissionRate);

		void setRainVelocity(int min, int max);

		void setSnowVelocity(int min, int max);

		void addWindVector(Ogre::Vector3 wind);

		void removeWindVector();

	private:
		Ogre::Root *cRoot;

		Ogre::SceneManager *cSceneMgr;

		Ogre::Camera *cCamera;

		Cloud *cWindCloud;

		Sun *cSun;

		Ogre::Real timer;

		Ogre::SceneNode *snowNode;

		Ogre::SceneNode *rainNode;

		Ogre::ParticleSystem *snowPS;

		Ogre::ParticleSystem *rainPS;

		Ogre::Vector3 *windVector;

		virtual bool frameStarted (const Ogre::FrameEvent &e);

		void updateComponents(Ogre::Real timeSinceLastFrame);

		void updateParticleFacing();

		void makeSky();

		void makeSun();

		void makeWindCloud();

		void updateSky();

		void updateSun();

		void updateWindCloud();
			
	};

}

#endif
