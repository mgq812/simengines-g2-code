#include "ProjectileCannon.h"
#include <NxOgre.h>
#include <NxOgreOGRE3D.h>
#include <iostream>
#include "NxPhysics.h"
#include "NxCooking.h"
//defines for different projectiles 
#define SHELL 1
#define GRENADE 2
#define BULLET 3 
#define FASTSHELL 4
#define OBLITERATE 5
//projectile struct, required for each grenade and shell.
struct projectile {
	short type; //type of the projectile. 
	OGRE3DBody*	mCube; //body assigned to the projectile.
	bool purge; // should the projectile be purged?
	Ogre::Real lifeTime; //How long has it been in existence -> required for grenades.
	NxVec3 location; //location of the projectile. 
};

//variable required for adding new shots to the list.
projectile shot;
//Vector that stores projectiles.
std::vector<projectile> pList;

//Cannon struct
struct cannon {
	int ID;
	NxOgre::Vec3 dir;
	NxOgre::Vec3 pos;
};

//variable for adding for new cannons.
cannon newCannon;
//Launcher list, list of all launchers.
std::vector<cannon> lList;
//ID for new launchers.
int IDGen = 1;

class myContactReport : public NxUserContactReport {
	void onContactNotify(NxContactPair& pair, NxU32 events) {
		//Stream iterator is away of getting what pairs collided and were
		NxContactStreamIterator i(pair.stream);
		while(i.goNextPair()) {
			for(unsigned int j = 0; j < pList.size(); j++) {
				//we look for collisions for any projectiles that are in action.
				if(pair.actors[0] == pList.at(j).mCube->getNxActor()) {
					//we set the location, to the location of the collision. 
					pList.at(j).location = pair.actors[0]->getGlobalPosition();
					//We set purge to true. This means that next time the purge method 
					//will be executed, the explosion will spawn.
					pList.at(j).purge = true;
				}
			}
		}
	}
};
//constructor.
ProjectileCannon::ProjectileCannon(OGRE3DRenderSystem* extRenderSystem) {
	projectileLife = 5; // default grenade life.
	std::vector<projectile> pList(16); // Default amount of projectiles in action*.
	exploSpawned = false; // no explosions are spawned.
	mRenderSystem = extRenderSystem; //set the render system. 
	//create kinenmatic actor to bind the explosion, also create the descriptor for the forcefield.
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	bodyDesc.flags |= NX_BF_KINEMATIC;
	bodyDesc.massSpaceInertia = NxVec3(1, 1, 1);
	bodyDesc.mass = 1.0f;
	actorDesc.body = &bodyDesc;
	nScene = mRenderSystem->getScene()->getScene();
	explosionHolder = nScene->createActor(actorDesc);
	
	cnt = 0;
	explosionLife = 0;
	grenadeLife = 3;
	launcherForce = 1000;
	exploForce = 250;
	//set the default contact report function.
	mRenderSystem->getScene()->getScene()->setUserContactReport(new myContactReport());
	defaultFfRadius = 10;
	//create the explosion. 
	explosion(mRenderSystem->getScene()->getScene(), explosionHolder );
	obliterated = false;
}
//see header file comments for descriptions.
void ProjectileCannon::setGrenadeLife(int tLife) {
	grenadeLife = tLife;
}
void ProjectileCannon::aimCannon(NxOgre::Vec3 direction, int ID) {
	lList.at(getLauncherIndex(ID)).dir = direction;
}
void ProjectileCannon::moveCannon(NxOgre::Vec3 position, int ID) {
	lList.at(getLauncherIndex(ID)).pos = position;
}

NxOgre::RaycastHit ProjectileCannon::fireBullet(int ID) {
	NxOgre::Ray ray;
	ray.mDirection = getLauncherDir(ID);
	ray.mOrigin = getLauncherPos(ID);
	//todo: bullet drop --> time = speed/distance
	//raycast again, with the height coordinate dropped by a value of d = -(1/2)g*t^2 
	//if(abs(oldDistance - newDistance) > epsilon) raycast again. 
	return mRenderSystem->getScene()->raycastClosestShape(ray, NxOgre::Enums::ShapesType_All);
}
void ProjectileCannon::fireFastShell(int ID) { 
	//we calculate the location of the raycast
	NxOgre::Ray ray;
	//grab the position of the starting launcher.
	ray.mDirection = getLauncherDir(ID);
	ray.mOrigin = getLauncherPos(ID);
	//shoot the ray.
	NxOgre::RaycastHit rayHit = mRenderSystem->getScene()->raycastClosestShape(ray, NxOgre::Enums::ShapesType_All);
	NxOgre::Vec3 loc = rayHit.mWorldImpact;
	//move the exlosion to the location, and change the radius. 
	gForceField->addShapeGroup(*m_inclusionGroup);
	NxMat34 pose;
	pose.id();
	pose.t = NxVec3(loc.x, loc.y, loc.z);
	explosionHolder->setGlobalPose(pose); 
	m_inclusionShape->setPose(pose);
	m_inclusionShape->isSphere()->setRadius(defaultFfRadius);
	exploSpawned = true;
}
void ProjectileCannon::fireShell(int ID) {
	//create a new projectile and add it to the list of projectiles.
	pList.reserve(1); // we reserve a spot.
	shot.mCube = mRenderSystem->createBody(new NxOgre::Box(0.2, 0.2, 0.2), getLauncherPos(ID), "cube.1m.mesh"); 
	shot.mCube->getEntity()->getParentNode()->scale(0.2, 0.2, 0.2);
	shot.mCube->setMass(0.1f);
	NxOgre::Matrix44 m44pose = Matrix44_Identity;
	//we add the force and set the appropriate contact flags. 
	shot.mCube->addForce(getLauncherDir(ID) * launcherForce ,NxOgre::Enums::ForceMode_Force, true);
	shot.mCube->getNxActor()->setContactReportFlags(NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_TOUCH | NX_NOTIFY_ON_END_TOUCH);
	//we set the other variables and add it to the end of the list.
	shot.purge = false;
	shot.lifeTime = 0;
	shot.type = SHELL;
	pList.insert(pList.end(), shot);

}
void ProjectileCannon::obliterate(int ID) {
	//huge force and radius.
	exploForce = 10000;
	defaultFfRadius = 1000;
	//create a new explosion. 
	explosion(mRenderSystem->getScene()->getScene(), explosionHolder);
	pList.reserve(1);
	shot.purge = true;
	shot.type = OBLITERATE;
	shot.mCube = NULL;
	NxOgre::Vec3 loc = getLauncherPos(ID);
	shot.location = NxVec3(loc.x, loc.y, loc.z);
	pList.insert(pList.end(), shot);
}
void ProjectileCannon::fireGrenade(int ID){ 
	//we create the objects.
	pList.reserve(1);
	//we create the grenade (cube.002.mesh) and set the variables for it.
	shot.mCube = mRenderSystem->createBody(new NxOgre::Box(0.82, 1.6, 0.6f), getLauncherPos(ID), "cube.002.mesh"); 
	NxOgre::Vec3 vector = getLauncherPos(ID);
	shot.mCube->getEntity()->getParentNode()->scale(0.2, 0.2, 0.2);
 	shot.mCube->setMass(0.5f);
	shot.mCube->addForce(getLauncherDir(ID) * launcherForce ,NxOgre::Enums::ForceMode_Force, true);
	//we still need contact reports, so we can update the life time of grenades. 
	shot.mCube->getNxActor()->setContactReportFlags(NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_TOUCH | NX_NOTIFY_ON_END_TOUCH);

	//other variables.
	shot.purge = false;
	shot.lifeTime = 0; //start life time.
	shot.type = GRENADE;
	pList.insert(pList.end(), shot);
}	
void ProjectileCannon::explosion(NxScene* scene, NxActor* actor) {
	NxForceFieldDesc ffDesc;
	NxForceFieldLinearKernelDesc	lKernelDesc;
	NxForceFieldLinearKernel*		linearKernel;

	//constant force of 100 outwards	
	lKernelDesc.constant = NxVec3(exploForce, 0, 0);

	//The forces do not depend on where the objects are positioned
	NxMat33 m;
	m.zero();
	lKernelDesc.positionMultiplier = m;
	lKernelDesc.noise = NxVec3(1,15,1); //adds a random noise on the forces to make the objects a little more chaotic

	//Set target velocity along the radius to 20
	lKernelDesc.velocityTarget = NxVec3(20,0,0);
	m.diagonal(NxVec3(1,0,0)); //Acts with a force relative to the current velocity to reach the
	//target velocities. 0 means that those components won't be affected
	lKernelDesc.velocityMultiplier = m;

	// create linear kernel
	linearKernel = scene->createForceFieldLinearKernel(lKernelDesc);
	ffDesc.kernel = linearKernel;

	//Attach the force field to an actor (kinematic) so that we can move it around 
	// (spawn the explosions in different places)
	ffDesc.actor = actor;

	//Create the force field around origo
	ffDesc.pose.id();
	ffDesc.coordinates = NX_FFC_SPHERICAL;
	ffDesc.flags = 0;

	gForceField = scene->createForceField(ffDesc);

	// inclusion group
	NxForceFieldShapeGroupDesc sgInclDesc;
	m_inclusionGroup = scene->createForceFieldShapeGroup(sgInclDesc);
	m_inclusionShape = NULL;
	NxSphereForceFieldShapeDesc s;
	s.radius = 0.1f;
	s.pose.t = NxVec3(0, 0, 0);
	m_inclusionShape = m_inclusionGroup->createShape(s);

	}
void ProjectileCannon::explode(int i) {
	NxMat34 pose;
	pose.id();
	pose.t = pList.at(i).location;
	explosionHolder->setGlobalPose(pose); // to set FF, could also get rid of that indirection				
	m_inclusionShape->setPose(pose);
	m_inclusionShape->isSphere()->setRadius(defaultFfRadius);
	//m_excludeShape->isSphere()->setPose(pose);
	//m_excludeShape->isSphere()->setRadius(0.1f);
	pList.erase(pList.begin() + i);
	exploSpawned = true;
}
void ProjectileCannon::purge(Ogre::Real evtTime) {	
	//remove all explosions that have been in existence for more than 50 ms.
	if(exploSpawned) {
		explosionLife += evtTime;
		if(explosionLife > 50) {
			gForceField->removeShapeGroup(*m_inclusionGroup);
			exploSpawned = false;
			explosionLife = 0;
		}
	}

	// we go through all projectiles in the list, and detonate any that have the flag set.
	for(unsigned int i = 0; i < pList.size(); i++) {
		pList.at(i).lifeTime += evtTime;
		if(pList.at(i).type == GRENADE) {
			//for grenades we also check if they've existed for more than grenadeLife seconds.
			if(pList.at(i).lifeTime > grenadeLife) {
				mRenderSystem->destroyBody(pList.at(i).mCube);
				gForceField->addShapeGroup(*m_inclusionGroup);
				explode(i);
				
			//	pList.erase(pList.begin() + i);
			}
		}
		//we remove them, we also remove projectiles that have been in existence to long
		//this is done so we don't get insane amounts of projectiles.
		else if(pList.at(i).purge || pList.at(i).lifeTime > projectileLife){
			mRenderSystem->destroyBody(pList.at(i).mCube);
			gForceField->addShapeGroup(*m_inclusionGroup);				
			explode(i);
		}
	}
}

int ProjectileCannon::addLauncher(NxOgre::Vec3 direction, NxOgre::Vec3 position) {
	lList.reserve(1);
	newCannon.dir = direction;
	newCannon.pos = position;
	newCannon.ID = IDGen;
	IDGen++;
	lList.insert(lList.end(), newCannon);
	return newCannon.ID;
}
int ProjectileCannon::removeLauncher(int ID) { 
	for(unsigned int i = 0; i < lList.size(); i++) {
		if(lList.at(i).ID == ID) {
			lList.erase(lList.begin() + i);
			return 1; // return 1 if found and erased
		}
	}
	//return 0 if error, wrong ID
	return 0;
}
//helper function. 
int ProjectileCannon::getLauncherIndex(int ID) {
	for(unsigned int i = 0; i < lList.size(); i++) {
		if(lList.at(i).ID == ID) {
			return i;
		}
	}
	//return 0 if error, wrong ID
	//there should be no IDs with the value of 0.
	return 0;
}

NxOgre::Vec3 ProjectileCannon::getLauncherPos(int ID) {
	return lList.at(getLauncherIndex(ID)).pos;
}
NxOgre::Vec3 ProjectileCannon::getLauncherDir(int ID) {
	return lList.at(getLauncherIndex(ID)).dir;
}