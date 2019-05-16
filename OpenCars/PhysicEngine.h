#pragma once
#include "PxPhysicsAPI.h"
#include <iostream>
#include "RigidBodyDynamic.h"
#include "RigidBodyStatic.h"

using namespace physx;
class PhysicEngine
{

private:
	PxPhysics *mSDK = nullptr;
	PxCooking *mCooking = nullptr;
	PxDefaultErrorCallback pDefaultErrorCallback;
	PxDefaultAllocator pDefaultAllocatorCallback;
	PxSimulationFilterShader pDefaultFilterShader = PxDefaultSimulationFilterShader;
	PxScene *mScene;

public:
	PhysicEngine();
	RigidBodyDynamic *createRigidBodyDynamic();
	RigidBodyStatic *createRigidBodyStatic();
	void simulate(float time);
	bool raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, PxRaycastBuffer &hit);
	void initScene();
	~PhysicEngine();
};

