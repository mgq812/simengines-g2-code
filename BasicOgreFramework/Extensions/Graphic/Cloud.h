#ifndef __Cloud_h_
#define __Cloud_h_

#include <ogre.h>

#include "InternalUtilities.h"
#include "Face.h"

namespace CartoonCaelum {

	class Cloud
	{

	public:
		Cloud(Ogre::SceneManager *sceneMgr, Ogre::Camera *camera,
			int xSize, int ySize, int distance);

		~Cloud();

		void setPosition(Ogre::Vector3 newPos);

		Ogre::SceneNode *getNode();

		Face* getFace();

	private:
		Ogre::SceneManager *cSceneMgr;

		Ogre::Camera *cCamera;

		Ogre::SceneNode *mainNode;

		Ogre::SceneNode *cloudNode;

		Ogre::SceneNode *faceNode;

		Ogre::Entity *cloudEntity;

		int cloudDistance;

		int cloudXSize;

		int cloudYSize;

		Face *cloudFace;

		Ogre::String uniqueSuffix;

		void createCloud(Ogre::String materialName);

	};

}

#endif
