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

	const Ogre::String InternalUtilities::pointerToString (void* pointer)
    {
        std::stringstream stream;
		stream.width(2 * sizeof(void *));
        stream.fill('0');
        stream.unsetf(std::ios::dec);
        stream.setf(std::ios::hex);
        stream.setf(std::ios::uppercase);
        stream << reinterpret_cast<ptrdiff_t>(pointer);
        return stream.str();
    }

}