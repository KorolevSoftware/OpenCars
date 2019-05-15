#pragma once
#include "PxPhysicsAPI.h"
#include <iostream>

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
	void simulate(float time);
	void initScene();
	~PhysicEngine();
};

