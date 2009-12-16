#ifndef __Crosshair_h_
#define __Crosshair_h_

#include <ogre.h>
#include <OgreStringConverter.h>
#include <OgreException.h>
#include <OgreMath.h>

class Crosshair: public Ogre::FrameListener
{

private:
	Ogre::SceneManager *cSceneMgr;

	Ogre::Camera *cCamera;

	Ogre::SceneNode *crossNode;

	Ogre::Entity *crossEntity;

	int crossDistance;

	int crossXSize;

	int crossYSize;

	virtual bool frameStarted (const Ogre::FrameEvent &e);

	void createCross(Ogre::String materialName);

public:
	Crosshair(Ogre::SceneManager *sceneMgr, Ogre::Camera *camera,
		int xSize, int ySize, int distance);
};

#endif
