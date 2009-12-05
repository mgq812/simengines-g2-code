//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_ECHO_H
#define OGRE_ECHO_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include <OgreAxisAlignedBox.h>
#include <OgreVector3.h>
#include <EchoProperties.h>
#include <vector>

using namespace std;

//A class for generating an echo which depends on the surroundings
class Echo : public Ogre::AxisAlignedBox
{
	public:	
		static EchoProperties Echo::calculateEcho(int volume, vector<Ogre::AxisAlignedBox> boundingBoxes, Ogre::Vector3 soundPosition);
};
//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||