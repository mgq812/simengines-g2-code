#ifndef __RotatingSkyObject_h_
#define __RotatingSkyObject_h_

#include <ogre.h>

#include "InternalUtilities.h"

namespace CartoonCaelum {
	
	/**
	 * Class representing sky object that rotates around the origin, providing methods for this rotation.
	 */
	class RotatingSkyObject
	{

	public:
		/**
		 * Constructor.
		 */
		RotatingSkyObject(Ogre::SceneNode *pNode, int nDistance, Ogre::Radian pitch);
		
		/**
		 * Destructor.
		 */
		virtual ~RotatingSkyObject();
		
		/**
		 * Rotates the sun around the origin, turning off the light it has gone down.
		 * @param degrees The degree of rotation.
		 */
		void moveObject(Ogre::Radian degrees);

	protected:
		/** Pointer to the scene node, which the object is attached to.*/
		Ogre::SceneNode *m_pObjectNode;
		
		/** The accumulated rotation that has previously been applied.*/
		Ogre::Radian m_previousRotation;
		
		/** The object's distance to the origin.*/
		int m_nObjectDistance;
		
		/** The degree which the rotation cycle has been pitched around the world's x-axis.*/
		Ogre::Radian m_cyclePitch;

	};

}

#endif
