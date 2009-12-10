// ===============================================================================
//						  NVIDIA PHYSX SDK TRAINING PROGRAMS
//						    LESSON 109 : COLLISION GROUPS
//
//						    Written by QA BJ, 6-2-2008
// ===============================================================================

#ifndef LESSON109_H
#define LESSON109_H

#include <GL/glut.h>
#include <stdio.h>

#include "NxPhysics.h"
#include "DrawObjects.h"
#include "HUD.h"

#include "Stream.h"
#include "NxCooking.h"

#include "DebugRenderer.h"

void PrintControls();
bool IsSelectable(NxActor* actor);
void SelectNextActor();
void ProcessCameraKeys();
void SetupCamera();
void RenderActors(bool shadows);
void DrawForce(NxActor* actor, NxVec3& forceVec, const NxVec3& color);

NxVec3 ApplyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength, bool forceMode);
void ProcessForceKeys();
void ProcessInputs();

void RenderCallback();
void ReshapeCallback(int width, int height);
void IdleCallback();
void KeyboardCallback(unsigned char key, int x, int y);
void KeyboardUpCallback(unsigned char key, int x, int y);
void SpecialCallback(int key, int x, int y);
void MouseCallback(int button, int state, int x, int y);
void MotionCallback(int x, int y);
void ExitCallback();
void InitGlut(int argc, char** argv);

NxActor* CreateBox(const NxVec3& pos, const NxVec3& boxDim, NxReal density);
NxActor* CreateSphere(const NxVec3& pos, NxReal radius, NxReal density);
NxActor* CreateCapsule(const NxVec3& pos, NxReal height, NxReal radius, NxReal density);

void SetActorCollisionGroup(NxActor* actor, NxCollisionGroup group);
void InitActorCollisionGroups();
void InitContactReports();
void InitContactReportsPerPair();
void InitContactReportsPerGroup();
void DisableCollisionResponse();


void InitializeHUD();

void InitNx();
void ReleaseNx();
void ResetNx();

void StartPhysics();
void GetPhysicsResults();

int main(int argc, char** argv);

#endif  // LESSON109_H


