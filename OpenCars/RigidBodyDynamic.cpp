#include "RigidBodyDynamic.h"

RigidBodyDynamic::RigidBodyDynamic(PxRigidDynamic *actor, PxPhysics *mSDK, PxCooking *mCooking)
{
	this->actor = actor;
	this->mSDK = mSDK;
	this->mCooking = mCooking;
	actor->setCMassLocalPose(PxTransform(PxVec3(1, 0, 0)));
}

void RigidBodyDynamic::addRelativeTorque(const glm::vec3 &torque)
{
	actor->addTorque(PxMat44(actor->getGlobalPose()).transform(PxVec3(torque.x, torque.y, torque.z)));
}

void RigidBodyDynamic::addTorque(const glm::vec3 &torque)
{
	actor->addTorque(PxVec3(torque.x, torque.y, torque.z));
}

void RigidBodyDynamic::addForceAtPos(const glm::vec3 &force, const glm::vec3 &origin)
{
	PxRigidBodyExt::addForceAtPos(*actor, PxVec3(force.x, force.y, force.z), PxVec3(origin.x, origin.y, origin.z));
}

void RigidBodyDynamic::addForce(const glm::vec3 &force)
{
	actor->addForce(PxVec3(force.x, force.y, force.z));
}

void RigidBodyDynamic::setBoxCollision(const glm::vec3 size)
{
	PxShape *shape;
	// Material
	PxMaterial *mMaterial = mSDK->createMaterial(0.0f, 0.0f, 0.1f);
	actor->setMass(10);
	shape = PxRigidActorExt::createExclusiveShape(*actor, PxBoxGeometry(size.x, size.y, size.z), *mMaterial);
	if (!shape)
		std::cerr << "create shape failed!";
}

void RigidBodyDynamic::setLocation(const glm::vec3 location)
{
	actor->setGlobalPose(PxTransform(PxVec3(location.x, location.y, location.z)));
}

void RigidBodyDynamic::setLinearVelocity(const glm::vec3 velosity)
{
	actor->setLinearVelocity(PxVec3(velosity.x, velosity.y, velosity.z));
}

void RigidBodyDynamic::setAngularVelocity(const glm::vec3 velosity)
{
	actor->setAngularVelocity(PxVec3(velosity.x, velosity.y, velosity.z));
}

glm::vec3 RigidBodyDynamic::getLinearVelocity()
{
	PxVec3 velocity = actor->getLinearVelocity();
	return glm::vec3(velocity.x, velocity.y, velocity.z);
}

glm::vec3 RigidBodyDynamic::getAngularVelocity()
{
	PxVec3 velocity = actor->getAngularVelocity();
	return glm::vec3(velocity.x, velocity.y, velocity.z);
}

glm::mat4 RigidBodyDynamic::getWorldMatrix()
{
	PxMat44 mt(actor->getGlobalPose());
	return glm::make_mat4(mt.front());
}


RigidBodyDynamic::~RigidBodyDynamic()
{
	if(actor)
		actor->release();
}
