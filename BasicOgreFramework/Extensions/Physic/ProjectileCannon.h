#include <NxOgre.h>
#include <NxOgreOGRE3D.h>
#include <iostream>
#include "NxPhysics.h"
#include "NxCooking.h"

class ProjectileCannon {
public:
	ProjectileCannon(OGRE3DRenderSystem* extRenderSystem, NxOgre::Vec3 direction, NxOgre::Vec3 position);
	void setGrenadeLife(int tLife);
	int getGreandeLife() { return grenadeLife; };
	void aimCannon(NxOgre::Vec3 direction, int ID);
	void moveCannon(NxOgre::Vec3 position, int ID);
	void purge(Ogre::Real evtTime);
	void fireShell(int ID);
	void fireGrenade(int ID);
	void fireFastShell(int ID);
	NxOgre::RaycastHit fireBullet(int ID);
	int addLauncher(NxOgre::Vec3 direction, NxOgre::Vec3 position);
	int removeLauncher(int ID);
	NxOgre::Vec3 getLauncherDir(int ID);
	NxOgre::Vec3 getLauncherPos(int ID);
	NxOgre::RaycastHit fireBullet();

	//getters setters implemented here
	int getLauncherForce() { return launcherForce; };
	void setLauncherForce(int lForce) { launcherForce = lForce; };

private:
	int getLauncherIndex(int ID);
	ProjectileCannon(){ };
	void explosion(NxScene* scene, NxActor* actor);
	void explode(int i);

protected:  
	NxScene* nScene;
	OGRE3DRenderSystem* mRenderSystem;
	//explosion forcefield variables
	bool exploSpawned;
	NxActor* explosionHolder;
	NxForceFieldShape* m_inclusionShape;
	NxForceFieldShape* m_excludeShape;
	NxForceFieldShapeGroup* m_excludeGroup;
	NxForceFieldShapeGroup* m_inclusionGroup;
	NxForceField* gForceField;
	//internal values
	int pSize, cnt, grenadeLife, launcherForce;
	Ogre::Real projectileLife, explosionLife;
	//projectile values
	NxOgre::Vec3 dir;
	NxOgre::Vec3 pos;
	OGRE3DBody*	mCube;
};