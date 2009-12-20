#ifndef __CartoonSystem_h_
#define __CartoonSystem_h_

#include <ogre.h>

#include "CartoonSkyObject.h"
#include "SkyObject.h"
#include "Sun.h"

namespace CartoonCaelum {
	
	/**
	 * Class that creates and updates cartoon sky, sky objects and weather effects.
	 * Implements a FrameListener, so it could be added as such to the Root in order
	 * to automatically update itself each frame. 
	 */
	class CartoonSystem: public Ogre::FrameListener 
	{

	public:
		/**
		 * Constructor.
		 */
		CartoonSystem(
			Ogre::Root *pRoot, 
			Ogre::SceneManager *pSceneMgr,
			Ogre::Camera *pCamera);
		
		/**
		 * Destructor.
		 */
		virtual ~CartoonSystem();

		void configure();
		
		/**
		 * Method that loads all needed resources into the resource manager.
		 * It will load all resources included in the CartoonResources.cfg file, by
		 * default added to the Cartoon resource subgroup.
		 */
		void setupResources();
		
		/**
		 * Method for initialising all loaded resources in the Cartoon resource subgroup.
		 */
		void loadResources();
		
		/**
		 *	Method that, if no rain particle system can be found, creates one and reapplies
		 *	the wind vector on all running particle systems.
		 */
		void addRain();
		
		/**
		 *	Method that, if no snow particle system can be found, creates one and reapplies
		 *	the wind vector on all running particle systems.
		 */
		void addSnow();
		
		/**
		 * Checks if CartoonSystem has a running rain particle system.
		 * @return true if rain particle system is not a nullpointer.
		 */
		bool hasRain();
		
		/**
		 * Checks if CartoonSystem has a running snow particle system.
		 * @return true if snow particle system is not a nullpointer.
		 */
		bool hasSnow();
		
		/**
		 * Removes the current rain particle system.
		 */
		void removeRain();
		
		/**
		 * Removes the current snow particle system.
		 */
		void removeSnow();
		
		/**
		 * Sets the emitter parameter emission_rate for all emmiters in the current 
		 * rain particle system.
		 * @param density The amount of particles emitted per second by each emitter.
		 */
		void setRainDensity(int nDensity);
		
		/**
		 * Sets the emitter parameter emission_rate for all emmiters in the current 
		 * snow particle system.
		 * @param density The amount of particles emitted per second by each emitter.
		 */
		void setSnowDensity(int nDensity);
		
		/**
		 * Sets the emitter parameters velocity_min and velocity_max for all emmiters
		 * in the current rain particle system.
		 * @param min The minimum initial velocity of each emitted particle.
		 * @param max The maximum initial velocity of each emitted particle.
		 */
		void setRainVelocity(int nMin, int nMax);
		
		/**
		 * Sets the emitter parameters velocity_min and velocity_max for all emmiters
		 * in the current snow particle system.
		 * @param min The minimum initial velocity of each emitted particle.
		 * @param max The maximum initial velocity of each emitted particle.
		 */
		void setSnowVelocity(int nMin, int nMax);
		
		/**
		 * Adds a vector representing wind direction to the CartoonSystem. The vector is
		 * stored and applied to all running particle systems. A cloud is also created in
		 * the opposite direction of the wind vector.
		 * @param wind The vector to set as the current wind vector.
		 */
		void addWindVector(Ogre::Vector3 vWind);
		
		/**
		 * Removes the current vector representing wind direction, and applies this to all
		 * running particle systems. The cloud in the opposite direction of the wind vector
		 * is also removed.
		 */
		void removeWindVector();

		/**
		 * Removes the current wind cloud.
		 */
		void removeWindCloud();
		
		/**
		 * Returns the current Sun object of the CartoonSystem.
		 * @return the current sun object.
		 */
		Sun* getSun();

	protected:
		/** Pointer to the main root object.*/
		Ogre::Root *m_pRoot;
		
		/** Pointer to the main scene manager object.*/
		Ogre::SceneManager *m_pSceneMgr;
		
		/** Pointer to the main camera object.*/
		Ogre::Camera *m_pCamera;
		
		/** Pointer to the Cloud object, representing a cloud that blows wind in the direction of the wind vector.*/
		CartoonSkyObject *m_pWindCloud;
		
		/** Pointer to the object representing the sun.*/
		Sun *m_pSun;
		
		/** Timer that keeps track on time, limiting updates per frame to updates per time unit instead.*/
		Ogre::Real m_fpTimer;

		/** Pointer to the scene node containing the current rain particle system.*/
		Ogre::SceneNode *m_pRainNode;
		
		/** Pointer to the scene node containing the current snow particle system.*/
		Ogre::SceneNode *m_pSnowNode;
		
		/** Array of pointers to scene nodes containing wind particle systems for the wind cloud.*/
		Ogre::SceneNode *m_paWindNode[5];
		
		/** Pointer to the current rain particle system.*/
		Ogre::ParticleSystem *m_pRainPS;
		
		/** Pointer to the current snow particle system.*/
		Ogre::ParticleSystem *m_pSnowPS;

		/** Array of pointers to the current wind particle systems.*/
		Ogre::ParticleSystem *m_paWindPS[5];
		
		/** Pointer to the current wind vector, representing wind direction.*/
		Ogre::Vector3 *m_pvWindVector;
		
		/**
		 * Creates the skydome for the CartoonSystem.
		 */
		void makeSky();
		
		/**
		 * Creates the sun object for the CartoonSystem.
		 */
		void makeSun();
		
		/**
		 * Creates the wind cloud object for the CartoonSystem,
		 * positioning it at the far horizon.
		 */
		void makeWindCloud();

		/** 
		 * Implementation of FrameListener method, run at the start of every frame.
		 * @param e the Frame»vent for the current frame.
		 */
		virtual bool frameStarted (const Ogre::FrameEvent &e);
		
		/**
		 * Method for performing updates on all currently active objects in the
		 * CartoonSystem.
		 * @param timeSinceLastFrame the amount of time since the last frame, in seconds.
		 */
		void updateComponents(Ogre::Real fpTimeSinceLastFrame);
		
		/**
		 * Updates the positioning of any active rain and snow particle systems with
		 * respect to the current camera location.
		 */
		void updateParticleFacing();
		
		/**
		 * Updates the sky dome of the CartoonSystem.
		 */
		void updateSky();
		
		/**
		 * Updates the movement of the sun and its face.
		 */
		void updateSun();
		
		/**
		 * Updates the movement of the wind cloud and its face.
		 */
		void updateWindCloud();
			
	};

}

#endif
