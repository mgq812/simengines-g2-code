#ifndef __Sun_h_
#define __Sun_h_

#include <ogre.h>

#include "InternalUtilities.h"
#include "CartoonSkyObject.h"
#include "RotatingSkyObject.h"
#include "Face.h"

namespace CartoonCaelum {
	
	/**
	 * Class representing a cartoon sun.
	 */
	class Sun : public CartoonSkyObject, public RotatingSkyObject
	{

	public:
		/**
		 * Constructor.
		 */
		Sun(Ogre::SceneManager *pSceneMgr, Ogre::Camera *pCamera, Ogre::SceneNode *pNode,
			int nXSize, int nYSize, int nDistance, Ogre::Radian pitch);
		
		/**
		 * Destructor.
		 */
		virtual ~Sun();
		
		/**
		 * Checks the position of the sun, and updates the light accordingly.
		 */
		void updateLight();
		
		/**
		 * Returns the light object for the sun.
		 * @return the light object.
		 */
		Ogre::Light* getLight();

	protected:
		/** Pointer to the scene node, which the light object is attached to.*/
		Ogre::SceneNode *m_pLightNode;
		
		/** Pointer to the light object.*/
		Ogre::Light *m_pSunLight;
		
		/**
		 * Method that creates the light object and sets its colour values.
		 */
		void createLight();

	};

}

#endif
