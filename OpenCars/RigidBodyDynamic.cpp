#include "RigidBodyDynamic.h"

RigidBodyDynamic::RigidBodyDynamic(PxRigidDynamic *actor, PxPhysics *mSDK, PxCooking *mCooking)
{
	this->actor = actor;
	this->mSDK = mSDK;
	this->mCooking = mCooking;
}

void RigidBodyDynamic::addRelativeTorque(glm::vec3 &torque)
{
	actor->addTorque(PxMat44(actor->getGlobalPose()).transform(PxVec3(torque.x, torque.y, torque.z)));
}

void RigidBodyDynamic::addTorque(glm::vec3 &torque)
{
	actor->addTorque(PxVec3(torque.x, torque.y, torque.z));
}

void RigidBodyDynamic::addForceAtPos(glm::vec3 &force, glm::vec3 &origin)
{
	PxRigidBodyExt::addForceAtPos(*actor, PxVec3(force.x, force.y, force.z), PxVec3(origin.x, origin.y, origin.z));
}

void RigidBodyDynamic::addForce(glm::vec3 &force)
{
	actor->addForce(PxVec3(force.x, force.y, force.z));
}


RigidBodyDynamic::~RigidBodyDynamic()
{
	if(actor)
		actor->release();
}
