#ifndef __Sun_h_
#define __Sun_h_

#include <ogre.h>

#include "InternalUtilities.h"
#include "Face.h"

namespace CartoonCaelum {

	class Sun
	{

	public:
		Sun(Ogre::SceneManager *sceneMgr, Ogre::Camera *camera,
			int xSize, int ySize, int distance, Ogre::Radian pitch);

		~Sun();

		void moveSun(Ogre::Radian degrees);

		Ogre::SceneNode *getNode();

		Face* getFace();

	private:
		Ogre::SceneManager *cSceneMgr;

		Ogre::Camera *cCamera;

		Face *sunFace;

		Ogre::SceneNode *mainNode;

		Ogre::SceneNode *sunNode;

		Ogre::SceneNode *lightNode;

		Ogre::Entity *sunEntity;

		Ogre::Light *sunLight;

		int sunDistance;

		int sunXSize;

		int sunYSize;

		Ogre::Radian previousRotation;

		Ogre::Radian cyclePitch;

		Ogre::String uniqueSuffix;

		void createSun(Ogre::String materialName);

		void createLight();

	};

}

#endif
