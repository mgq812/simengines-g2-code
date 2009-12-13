#ifndef __InternalUtilities_h_
#define __InternalUtilities_h_

#include <ogre.h>
#include <OgreStringConverter.h>
#include <OgreException.h>
#include <OgreMath.h>

namespace CartoonCaelum {

	class InternalUtilities 
	{
		public:
			static int round(Ogre::Real number);
	};

}

#endif