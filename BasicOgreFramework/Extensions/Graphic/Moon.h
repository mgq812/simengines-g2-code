#ifndef __Moon_h_
#define __Moon_h_

#include <ogre.h>

#include "InternalUtilities.h"
#include "CartoonSkyObject.h"
#include "RotatingSkyObject.h"
#include "Face.h"

namespace CartoonCaelum {
	
	/** Enumeration of all moon phases.*/
	enum MoonPhase {FULL, WAXING_GIBBOUS, FIRST_QUARTER, WAXING_CRESCENT, 
		NEW, WANING_CRESCENT, THIRD_QUARTER, WANING_GIBBOUS};
	
	/**
	 * Class representing a cartoon moon.
	 */
	class Moon : public CartoonSkyObject, public RotatingSkyObject
	{

	public:
		/**
		 * Constructor.
		 */
		Moon(Ogre::SceneManager *pSceneMgr, Ogre::Camera *pCamera, Ogre::SceneNode *pNode,
			int nXSize, int nYSize, int nDistance, Ogre::Radian pitch, MoonPhase eCurPhase);
		
		/**
		 * Destructor.
		 */
		virtual ~Moon();

		/**
		 * Checks the position of the moon, and updates the light accordingly.
		 */
		void updateLight();
		
		/**
		 * Returns the material name currently corresponding to the moon phase.
		 * @param phase The moon phase for which to return the corresponding material name.
		 * @return The material name corresponding to the moon phase.
		 */
		Ogre::String getPhaseMaterial(MoonPhase phase);
		
		/**
		 * Sets a new material name for the given moon phase.
		 * @param phase The moon phase for which to set a new material name.
		 * @param material The new material name to set.
		 */
		void setPhaseMaterial(MoonPhase phase, Ogre::String material);
		
		/**
		 * Returns the face material name currently corresponding to the moon phase.
		 * @param phase The moon phase for which to return the corresponding face material name.
		 * @return The face material name corresponding to the moon phase.
		 */
		Ogre::String getFaceMaterial(MoonPhase phase);
		
		/**
		 * Sets a new face material name for the given moon phase.
		 * @param phase The moon phase for which to set a new face material name.
		 * @param material The new face material name to set.
		 */
		void setFaceMaterial(MoonPhase phase, Ogre::String material);
		
		/**
		 * Sets the current moon phase
		 * @param phase The new moon phase.
		 */
		void setPhase(MoonPhase phase);
		
		/**
		 * Returns the light object for the moon.
		 * @return the light object.
		 */
		Ogre::Light* getLight();

	protected:
		/** Pointer to the scene node, which the light object is attached to.*/
		Ogre::SceneNode *m_pLightNode;
		
		/** Pointer to the light object.*/
		Ogre::Light *m_pMoonLight;
		
		/** The current phase of the moon.*/
		MoonPhase m_eCurPhase;

		/** The material names corresponding to the different moon phases.*/
		Ogre::String m_straPhaseMaterials[8];
		
		/** The material names of the faces corresponding to the different moon phases.*/
		Ogre::String m_straFaceMaterials[8];
		
		/**
		 * Method that creates the light object and sets its colour values.
		 */
		void createLight();
		
		/**
		 * Sets the default material names in the phase material array.
		 */
		void setDefaultMaterials();

	};

}

#endif
