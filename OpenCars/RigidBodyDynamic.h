#pragma once
#include "PxPhysicsAPI.h"
#include <glm/glm.hpp>

using namespace physx;

class RigidBodyDynamic
{

private:
	PxPhysics *mSDK = nullptr;
	PxCooking *mCooking = nullptr;
	PxRigidDynamic * actor = nullptr;

public:
	RigidBodyDynamic(PxRigidDynamic *actor, PxPhysics *mSDK, PxCooking *mCooking);
	void addRelativeTorque(glm::vec3 &torque);
	void addTorque(glm::vec3 &torque);
	void addForceAtPos(glm::vec3 &force, glm::vec3 &origin);
	void addForce(glm::vec3 &force);
	~RigidBodyDynamic();
};

