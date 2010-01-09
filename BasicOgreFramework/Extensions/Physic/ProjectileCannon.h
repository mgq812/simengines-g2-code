#include <NxOgre.h>
#include <NxOgreOGRE3D.h>
#include <iostream>
#include "NxPhysics.h"
#include "NxCooking.h"
/** @brief Main class for managing projectiles. 
*/
class ProjectileCannon {
public:
	/** Default constructor. Required before further use of the class. 
	*@param extRenderSystem - OGRE3DRenderSystem 
	*/
	ProjectileCannon(OGRE3DRenderSystem* extRenderSystem);
	/** Sets the time before the grenade will detonate.
	*@param tLife - Time in seconds.
	*/
	void setGrenadeLife(int tLife);
	/** Retuns the grenade life in seconds.
	*@return Grenade life in seconds.
	*/
	int getGreandeLife() { return grenadeLife; };
	/** Aims the cannon with the ID in the specified direction.
	*@param direction - The direction towards where the cannon should be aimed.
	*@param ID - ID of the cannon we're aiming. 
	*/
	void aimCannon(NxOgre::Vec3 direction, int ID);
	/** Moves the cannon with the ID in the specified direction.
	*@param position - The position of where we should place the cannon.
	*@param ID - ID of the cannon we're moving.
	*/
	void moveCannon(NxOgre::Vec3 position, int ID);
	/** Purge method. Required to be called each frame refresh.
	*@param evtTime - Time passed before last call of the method in ms.
	*/
	void purge(Ogre::Real evtTime);
	/** Fires a shell type projectile from the cannon with the ID.
	*@param ID - ID of the cannon we're firing.
	*/
	void fireShell(int ID);
	/** Fires a grenade type projectile from the cannon with the ID.
	*@param ID - ID of the cannon we're firing.
	*/
	void fireGrenade(int ID);
	/** Fires a fast shell type projectile from the cannon with the ID.
	*@param ID - ID of the cannon we're firing.
	*/
	void fireFastShell(int ID);
	/** Experimental/joke method. Creates a huge explosion at the origin of the ID cannon.
	*@param ID - ID of the cannon.
	*/
	void obliterate(int ID);
	/** Fires a raycast (bullet) from the cannon with the ID. 
	*@param ID - ID of the cannon.
	*@return Returns the raycast result.
	*/
	NxOgre::RaycastHit fireBullet(int ID);
	/** Adds a launcher to the manager.
	*@param direction - Default direction of the launcher.
	*@param position - Default position of the launcher.
	*@return ID of the launcher created.
	*/
	int addLauncher(NxOgre::Vec3 direction, NxOgre::Vec3 position);
	/** Removes a launcher from the manager.
	*@param ID - ID of the launcher to be removed.
	*@return Result of the operation. 
	*/
	int removeLauncher(int ID);
	/** Retruns the direction of the launcher.
	*@param ID - ID of the launcher to be returned.
	*@return Direction of the launcher.
	*/
	NxOgre::Vec3 getLauncherDir(int ID);
	/** Retruns the position of the launcher.
	*@param ID - ID of the launcher to be returned.
	*@return Position of the launcher.
	*/
	NxOgre::Vec3 getLauncherPos(int ID);
	/** Returns the force, that launcher projectiles.
	*@return Value of the force.
	*/
	int getLauncherForce() { return launcherForce; };
	/** Sets the force with which we launch the projectiles.
	*@param lForce - Force to be set.
	*/
	void setLauncherForce(int lForce) { launcherForce = lForce; };

private:
	/** Internal method. Returns the launcher index in the vector.
	*@param ID - ID of the launcher.
	*@return Returns the index of the launcher.
	*/
	int getLauncherIndex(int ID);
	/** Internal method. Creates an explosion explosion force field.
	*@param scene - Default NxScene.
	*@param actor - Actor to which the force field is attached.
	*/
	void explosion(NxScene* scene, NxActor* actor);
	/** Creates an explosion at the launcher at the index i.
	*@param i - Index of the launcher in the vector.
	*/
	void explode(int i);

protected:  
	int exploForce; /**< Force of the explosion */
	int defaultFfRadius; /**< Default radius of the force field. */
	NxScene* nScene; /**< global NxScene variable. */
	OGRE3DRenderSystem* mRenderSystem; /**< OGRE Render System*/
	//explosion forcefield variables
	bool exploSpawned; /**< boolean variable indicating if an explosion was spawned recently.*/
	bool obliterated; /**< boolean variable indicating if an obliteration was triggered */
	NxActor* explosionHolder; /**< NxActor that holds the explosion force field.*/
	// Force field shape descriptors.
	NxForceFieldShape* m_inclusionShape; 
	NxForceFieldShape* m_excludeShape; 
	NxForceFieldShapeGroup* m_excludeGroup;
	NxForceFieldShapeGroup* m_inclusionGroup;
	NxForceField* gForceField; /**< force field variable */
	//internal valuess
	int cnt, grenadeLife, launcherForce;
	Ogre::Real projectileLife, explosionLife; 
	//projectile values
	OGRE3DBody*	mCube; /**< internal variable for the projectile fired.*/
};