#include "ProjectileCannon.h"
#include <NxOgre.h>
#include <NxOgreOGRE3D.h>
#include <iostream>
#include "NxPhysics.h"
#include "NxCooking.h"

#define SHELL 1
#define GRENADE 2
#define BULLET 3 
#define FASTSHELL 4

struct projectile {
	short type;
	//unused atm
	OGRE3DBody*	mCube;
	bool purge;
	Ogre::Real lifeTime;
	NxVec3 location;
};

projectile shot;
std::vector<projectile> pList;

struct cannon {
	int ID;
	NxOgre::Vec3 dir;
	NxOgre::Vec3 pos;
};

cannon newCannon;
std::vector<cannon> lList;
int IDGen = 1;

class myContactReport : public NxUserContactReport {
	void onContactNotify(NxContactPair& pair, NxU32 events) {
		//Stream iterator is away of getting what pairs collided and were
		NxContactStreamIterator i(pair.stream);
		// Read the Physx Doc about this, its pretty logical
		while(i.goNextPair()) {
			//pair.actors[0]->
			for(unsigned int j = 0; j < pList.size(); j++) {
				if(pair.actors[0] == pList.at(j).mCube->getNxActor()) {
					pList.at(j).location = pair.actors[0]->getGlobalPosition();
					//pair.actors[0]->setGlobalPosition(pair.actors[0]->getGlobalPosition()+ NxVec3(10000));
					pList.at(j).purge = true;
				}
			}
		}
	}
};
ProjectileCannon::ProjectileCannon(OGRE3DRenderSystem* extRenderSystem, NxOgre::Vec3 direction, NxOgre::Vec3 position) {
	dir = direction;
	pos = position;
	pSize = 1;
	projectileLife = 5; 
	std::vector<projectile> pList(16);
	exploSpawned = false;
	mRenderSystem = extRenderSystem;
	//create kinenmatic actor to bind the explosion too
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;
	bodyDesc.flags |= NX_BF_KINEMATIC;
	bodyDesc.massSpaceInertia = NxVec3(1, 1, 1);
	bodyDesc.mass = 1.0f;
	actorDesc.body = &bodyDesc;
	nScene = mRenderSystem->getScene()->getScene();
	explosionHolder = nScene->createActor(actorDesc);
	explosion(mRenderSystem->getScene()->getScene(), explosionHolder );
	cnt = 0;
	explosionLife = 0;
	grenadeLife = 3;
	launcherForce = 1000;

	mRenderSystem->getScene()->getScene()->setUserContactReport(new myContactReport());
}
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
	//if(abs(oldDistance - newDistance) > 0.5) raycast again. 
	return mRenderSystem->getScene()->raycastClosestShape(ray, NxOgre::Enums::ShapesType_All);
}
void ProjectileCannon::fireFastShell(int ID) { 
	NxOgre::Ray ray;
	ray.mDirection = getLauncherDir(ID);
	ray.mOrigin = getLauncherPos(ID);
	NxOgre::RaycastHit rayHit = mRenderSystem->getScene()->raycastClosestShape(ray, NxOgre::Enums::ShapesType_All);
	pList.reserve(1);
	shot.purge = true;
	shot.type = FASTSHELL;
	shot.mCube = NULL;
	NxOgre::Vec3 loc = rayHit.mWorldImpact;
	shot.location = NxVec3(loc.x, loc.y, loc.z);
	pList.insert(pList.end(), shot);
}
void ProjectileCannon::fireShell(int ID) {
	pList.reserve(1);
	shot.mCube = mRenderSystem->createBody(new NxOgre::Box(1, 1, 1), getLauncherPos(ID), "cube.1m.mesh"); 
	shot.mCube->setMass(0.1f);
	shot.mCube->addForce(getLauncherDir(ID) * launcherForce ,NxOgre::Enums::ForceMode_Force, true);
	shot.mCube->getNxActor()->setContactReportFlags(NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_TOUCH | NX_NOTIFY_ON_END_TOUCH);

	shot.purge = false;
	shot.lifeTime = 0;
	shot.type = SHELL;
	pList.insert(pList.end(), shot);

}
void ProjectileCannon::fireGrenade(int ID){ 

	pList.reserve(1);
	shot.mCube = mRenderSystem->createBody(new NxOgre::Box(1, 1, 1), getLauncherPos(ID), "cube.1m.mesh"); 
	shot.mCube->setMass(0.5f);
	shot.mCube->addForce(getLauncherDir(ID) * launcherForce ,NxOgre::Enums::ForceMode_Force, true);
	shot.mCube->getNxActor()->setContactReportFlags(NX_NOTIFY_ON_START_TOUCH | NX_NOTIFY_ON_TOUCH | NX_NOTIFY_ON_END_TOUCH);

	shot.purge = false;
	shot.lifeTime = 0;
	shot.type = GRENADE;
	pList.insert(pList.end(), shot);
}	
void ProjectileCannon::explosion(NxScene* scene, NxActor* actor) {
	NxForceFieldDesc ffDesc;
	NxForceFieldLinearKernelDesc	lKernelDesc;
	NxForceFieldLinearKernel*		linearKernel;

	//constant force of 100 outwards	
	lKernelDesc.constant = NxVec3(250, 0, 0);

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

	// exclude group\q
	/*NxForceFieldShapeGroupDesc sgExclDesc;
	sgExclDesc.flags = NX_FFSG_EXCLUDE_GROUP;
	m_excludeGroup = scene->createForceFieldShapeGroup(sgExclDesc);
	NxSphereForceFieldShapeDesc exclude;
	exclude.radius = 0.2f;
	exclude.pose.t = NxVec3(0, 0, 0);
	m_excludeShape = m_excludeGroup->createShape(exclude);

	gForceField->addShapeGroup(*m_excludeGroup);*/
}
void ProjectileCannon::explode(int i) {
	NxMat34 pose;
	pose.id();
	pose.t = pList.at(i).location;
	explosionHolder->setGlobalPose(pose); // to set FF, could also get rid of that indirection				
	m_inclusionShape->setPose(pose);
	m_inclusionShape->isSphere()->setRadius(10);
	//m_excludeShape->isSphere()->setPose(pose);
	//m_excludeShape->isSphere()->setRadius(0.1f);
	pList.erase(pList.begin() + i);
	exploSpawned = true;
}
void ProjectileCannon::purge(Ogre::Real evtTime) {

	if(exploSpawned) {
		explosionLife += evtTime;
		if(explosionLife > 100) {
			m_inclusionShape->isSphere()->setRadius(0.1f);
	//		m_excludeShape->isSphere()->setRadius(0.2f);
			gForceField->removeShapeGroup(*m_inclusionGroup);
			exploSpawned = false;
			//explosion(mRenderSystem->getScene()->getScene(), explosionHolder );
			explosionLife = 0;
		}
	}

	for(unsigned int i = 0; i < pList.size(); i++) {
		pList.at(i).lifeTime += evtTime;
		if(pList.at(i).type == GRENADE) {
			if(pList.at(i).lifeTime > grenadeLife) {
				mRenderSystem->destroyBody(pList.at(i).mCube);
				gForceField->addShapeGroup(*m_inclusionGroup);
				explode(i);
			}
		}
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
int ProjectileCannon::getLauncherIndex(int ID) {
	for(unsigned int i = 0; i < lList.size(); i++) {
		if(lList.at(i).ID == ID) {
			return i;
		}
	}
	//return 0 if error, wrong ID
	return 0;
}

NxOgre::Vec3 ProjectileCannon::getLauncherPos(int ID) {
	return lList.at(getLauncherIndex(ID)).pos;
}
NxOgre::Vec3 ProjectileCannon::getLauncherDir(int ID) {
	return lList.at(getLauncherIndex(ID)).dir;
}