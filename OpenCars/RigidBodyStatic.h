#pragma once
#include "PxPhysicsAPI.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
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
	glm::mat4 getWorldMatrix();
	~RigidBodyStatic();
};

