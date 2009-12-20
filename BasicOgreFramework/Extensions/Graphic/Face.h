#ifndef __Face_h_
#define __Face_h_

#include <ogre.h>

#include "InternalUtilities.h"
#include "SkyObject.h"

namespace CartoonCaelum {
	
	/**
	 * Class representing a smiley face on a plane mesh.
	 */
	class Face : public SkyObject
	{
	
	public:
		/**
		 * Constructor.
		 */
		Face(Ogre::SceneManager *pSceneMgr, Ogre::Camera *pCamera, Ogre::SceneNode *pNode,
			 Ogre::String strMaterialName, int nXSize, int nYSize, int nDistance);
		
		/**
		 * Destructor.
		 */
		virtual ~Face();
		
		/**
		 * Rotate the face so that its up vector corresponds to the up vector of the camera.
		 */
		void directFace();

	};

}

#endif
