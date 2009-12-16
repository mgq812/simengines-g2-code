#ifndef __Sun_h_
#define __Sun_h_

#include <ogre.h>

#include "InternalUtilities.h"

namespace CartoonCaelum {

	class Sun
	{

	public:
		Sun(Ogre::SceneManager *sceneMgr, Ogre::Camera *camera,
			int xSize, int ySize, int distance, Ogre::Radian pitch);

		void setMood(Ogre::String materialName);

		void moveSun(Ogre::Radian degrees);

		void directMood();

	private:
		Ogre::SceneManager *cSceneMgr;

		Ogre::Camera *cCamera;

		Ogre::SceneNode *sunNode;

		Ogre::SceneNode *moodNode;

		Ogre::SceneNode *lightNode;

		Ogre::Entity *sunEntity;

		Ogre::Entity *moodEntity;

		Ogre::Light *sunLight;

		int sunDistance;

		int sunXSize;

		int sunYSize;

		Ogre::Radian previousRotation;

		Ogre::Radian cyclePitch;

		void createSun(Ogre::String materialName);

		void createMood();

		void createLight();

	};

}

#endif
