#ifndef __Sun_h_
#define __Sun_h_

#include <ogre.h>

#include "InternalUtilities.h"
#include "CartoonSkyObject.h"
#include "Face.h"

namespace CartoonCaelum {
	
	/**
	 * Class representing a cartoon sun.
	 */
	class Sun : public CartoonSkyObject
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
		 * Rotates the sun around the origin, turning off the light if y-value is below 0.
		 * @param degrees The degree of rotation.
		 */
		void moveSun(Ogre::Radian degrees);
		
		/**
		 * Returns the light object for the sun.
		 * @return the light object.
		 */
		Ogre::Light* getLight();
		
		/**
		 * Returns true if the light setting has been changed - that is if the light has been
		 * turned on or off.
		 */
		bool isLightChanged();
		
		/**
		 * Sets mLightChanged to true if it was false, and false if it was true.
		 */
		void flipLightChanged();

	private:
		/** Pointer to the scene node, which the light object is attached to.*/
		Ogre::SceneNode *m_pLightNode;
		
		/** Pointer to the light object.*/
		Ogre::Light *m_pSunLight;
		
		/** The accumulated rotation that has previously been applied to the sun.*/
		Ogre::Radian m_previousRotation;
		
		/** The degree which the sun's rotation cycle has been pitched around the world's x-axis.*/
		Ogre::Radian m_cyclePitch;
		
		/** True if the light has just been turned on or off.*/
		bool m_fLightChanged;
		
		/**
		 * Method that creates the light object and sets its colour values.
		 */
		void createLight();

	};

}

#endif
