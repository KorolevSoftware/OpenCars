#pragma once
#include "PxPhysicsAPI.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
using namespace physx;

class RigidBodyDynamic
{

private:
	PxPhysics *mSDK = nullptr;
	PxCooking *mCooking = nullptr;
	PxRigidDynamic * actor = nullptr;

public:
	RigidBodyDynamic(PxRigidDynamic *actor, PxPhysics *mSDK, PxCooking *mCooking);
	void addRelativeTorque(const glm::vec3 &torque);
	void addTorque(const glm::vec3 &torque);
	void addForceAtPos(const glm::vec3 &force, const glm::vec3 &origin);
	void addForce(const glm::vec3 &force);
	void setBoxCollision(const glm::vec3 size);
	void setLocation(const glm::vec3 location);
	void setLinearVelocity(const glm::vec3 velosity);
	void setAngularVelocity(const glm::vec3 velosity);
	glm::vec3 getLinearVelocity();
	glm::vec3 getAngularVelocity();
	glm::mat4 getWorldMatrix();

	~RigidBodyDynamic();
};

