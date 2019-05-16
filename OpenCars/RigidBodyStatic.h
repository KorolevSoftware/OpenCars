#pragma once
#include "PxPhysicsAPI.h"
#include <glm/glm.hpp>
#include <vector>

using namespace physx;


class RigidBodyStatic
{
private:
	PxPhysics *mSDK = nullptr;
	PxCooking *mCooking = nullptr;
	PxRigidStatic * actor;

public:
	RigidBodyStatic(PxRigidStatic * actor, PxPhysics *mSDK, PxCooking *mCooking);
	void setTraiangleMesh(std::vector<float> &rawVertex);
	~RigidBodyStatic();
};

