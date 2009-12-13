#include "InternalUtilities.h"

using namespace Ogre;

namespace CartoonCaelum {
	
	int InternalUtilities::round(Real number)
	{
		if (number < 0.5) {
			return Ogre::Math::IFloor(number);
		} else {
			return Ogre::Math::ICeil(number);
		}
	}

}