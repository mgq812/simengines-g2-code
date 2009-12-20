#ifndef __CartoonSkyObject_h_
#define __CartoonSkyObject_h_

#include <ogre.h>

#include "SkyObject.h"
#include "Face.h"

namespace CartoonCaelum {
	
	/**
	 * Class representing a sky object that has a smiley face on it.
	 */
	class CartoonSkyObject : public SkyObject
	{

	public:
		/**
		 * Constructor.
		 */
		CartoonSkyObject(Ogre::SceneManager *pSceneMgr, Ogre::Camera *pCamera,
			Ogre::SceneNode *pNode,Ogre::String strMaterialName, Ogre::String strFaceMaterialName, 
			int nXSize, int nYSize, int nDistance, Ogre::Real fpFaceXRatio, Ogre::Real fpFaceYRatio);
		
		/**
		 * Destructor.
		 */
		virtual ~CartoonSkyObject();
		
		/**
		 * Return the face object for this cartoon sky object.
		 * @return the face object for this cartoon sky object.
		 */
		Face* getFace();

	protected:
		/** Pointer to the face object.*/
		Face *m_pFace;

	};

}

#endif
