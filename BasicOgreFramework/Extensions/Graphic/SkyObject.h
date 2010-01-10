#ifndef __SkyObject_h_
#define __SkyObject_h_

#include <ogre.h>

#include "InternalUtilities.h"

namespace CartoonCaelum {
	
	/**
	 * Class representing a sky object consisting of a material on a plane mesh.
	 */
	class SkyObject
	{

	public:
		/**
		 * Constructor.
		 */
		SkyObject(Ogre::SceneManager *pSceneMgr, Ogre::Camera *pCamera,
			Ogre::SceneNode *pNode, Ogre::String strMaterialName, 
			int nXSize, int nYSize, int nDistance);
		
		/**
		 * Destructor.
		 */
		virtual ~SkyObject();
		
		/**
		 * Set the position of the object by a vector that directs the object from
		 * the origin; the mDistance variable determines the magnitude of the vector.
		 * @param newPos directional vector.
		 */
		void setPosition(Ogre::Vector3 vNewPos);
		
		/**
		 * Return the main scene node of this object.
		 * @return the main scene node.
		 */
		Ogre::SceneNode *getNode();
		
		/**
		 * Return string containing the name of the currently used material.
		 * @return string with material name.
		 */
		Ogre::String getCurrentMaterial();

		/**
		 * Sets the material on the plane mesh.
		 * @param materialName string with material name.
		 */
		void setCurrentMaterial(Ogre::String strMaterialName);
		
		/**
		 * Return the distance to the origin.
		 * @return The distance to the origin.
		 */
		int getDistance();
		
		/**
		 * Return the size of the object x-wise.
		 * @return The x-size of the origin.
		 */
		int getXSize();
		
		/**
		 * Return the size of the object y-wise.
		 * @return The y-size of the origin.
		 */
		int getYSize();
		
		/**
		 * Rotate the entity node so that its up vector corresponds to the up vector of the camera.
		 */
		void directEntity();
		
		/**
		 * Rotate the main node so that its up vector corresponds to the up vector of the camera.
		 */
		void directObject();

	protected:
		/** Pointer to the main scene manager object.*/
		Ogre::SceneManager *m_pSceneMgr;

		/** Pointer to the main camera object.*/
		Ogre::Camera *m_pCamera;

		/** Pointer to the main scene node.*/
		Ogre::SceneNode *m_pMainNode;

		/** Pointer to the entity node, which the plane entity is attached to.*/
		Ogre::SceneNode *m_pEntityNode;

		/** Pointer to the entity, containing the plane.*/
		Ogre::Entity *m_pMainEntity;

		/** The object's distance to the origin.*/
		int m_nDistance;
		
		/** The x-size in pixels of the plane mesh.*/
		int m_nXSize;

		/** The y-size in pizels of the plane mesh.*/
		int m_nYSize;
		
		/** The generated unique suffix for this object.*/
		Ogre::String m_strUniqueSuffix;
		
		/** The currently used material name; "" will render the object invisible.*/
		Ogre::String m_strMaterialName;

		/**
		 *	Rotate the node so that its up vector corresponds to the up vector of the camera.
		 */
		void directNode(Ogre::SceneNode *node);
		
		/**
		 * Method that creates the plane mesh, and sets up its entity and scene node.
		 */
		void createBody();

	};

}

#endif
