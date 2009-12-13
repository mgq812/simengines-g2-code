#ifndef __CartoonSystem_h_
#define __CartoonSystem_h_

#include <ogre.h>
#include <OgreStringConverter.h>
#include <OgreException.h>
#include "Sun.h"

namespace CartoonCaelum {

	class CartoonSystem: public Ogre::FrameListener 
	{

	private:
		Ogre::Root *cRoot;

		Ogre::SceneManager *cSceneMgr;

		Ogre::Camera *cCamera;

		Sun *cSun;

		Ogre::Real *timer;

		Ogre::SceneNode *snowNode[8];

		Ogre::ParticleSystem *snowPS[8];

		bool hasWindVector;

		virtual bool frameStarted (const Ogre::FrameEvent &e);

		void updateComponents(Ogre::Real timeSinceLastFrame);

		void updateParticleFacing();

		void makeSky();

		void makeSun();

		void makeWeather();

		void updateSky();

		void updateSun();

	public:
		CartoonSystem(
			Ogre::Root *root, 
			Ogre::SceneManager *sceneMgr,
			Ogre::Camera *camera);

		void configure();

		void setupResources();

		void loadResources();

		void setWindVector(Ogre::Vector3 wind);
			
	};

}

#endif
