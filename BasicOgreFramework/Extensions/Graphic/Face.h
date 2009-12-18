#ifndef __Face_h_
#define __Face_h_

#include <ogre.h>

#include "InternalUtilities.h"

namespace CartoonCaelum {

	class Face
	{

	public:
		Face(Ogre::SceneManager *sceneMgr, Ogre::Camera *camera,
			Ogre::SceneNode *node, int xSize, int ySize, int distance);

		~Face();

		void setFace(Ogre::String materialName);

		void directFace();

		Ogre::SceneNode* getNode();

		Ogre::String getCurrentFace();

	private:
		Ogre::SceneManager *cSceneMgr;

		Ogre::Camera *cCamera;

		Ogre::SceneNode *faceNode;

		Ogre::Entity *faceEntity;

		int faceDistance;

		int faceXSize;

		int faceYSize;

		Ogre::String uniqueSuffix;

		Ogre::String currentFace;

		void createFace();

	};

}

#endif
