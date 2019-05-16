#include "PhysicEngine.h"


PhysicEngine::PhysicEngine()
{
	PxFoundation *mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, pDefaultAllocatorCallback, pDefaultErrorCallback);
	mSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, PxTolerancesScale(), true);
	PxTolerancesScale scale;

	if (!scale.isValid())
		return;

	PxCookingParams params(scale);
	params.meshWeldTolerance = 0.001f;
	params.meshPreprocessParams = PxMeshPreprocessingFlags(PxMeshPreprocessingFlag::eWELD_VERTICES);

	mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, PxCookingParams(scale));
	PxPvd *gPvd = PxCreatePvd(*mFoundation);
	if (!mSDK)
	{
		std::cerr << "An error has happened." << std::endl;
		exit(1);
	}

	if (!PxInitExtensions(*mSDK, gPvd))
	{
		std::cerr << "An error has happened." << std::endl;
		exit(1);
	}
}

RigidBodyDynamic *PhysicEngine::createRigidBodyDynamic()
{
	PxTransform pose = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	// Plane
	PxRigidDynamic *body = mSDK->createRigidDynamic(pose);
	mScene->addActor(*body);
	return new RigidBodyDynamic(body, mSDK, mCooking);
}

RigidBodyStatic * PhysicEngine::createRigidBodyStatic()
{
	PxTransform pose = PxTransform(PxVec3(0.0f, 0.0f, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));
	// Plane
	PxRigidStatic *body = mSDK->createRigidStatic(pose);
	mScene->addActor(*body);
	return new RigidBodyStatic(body, mSDK, mCooking);
}

void PhysicEngine::simulate(float time)
{
	mScene->simulate(time);
	mScene->fetchResults(true);
}

bool PhysicEngine::raycast(glm::vec3 origin, glm::vec3 direction, float maxDistance, PxRaycastBuffer &hit)
{
	return mScene->raycast(PxVec3(origin.x, origin.y, origin.z), PxVec3(direction.x, direction.y, direction.z), maxDistance, hit);
}

void PhysicEngine::initScene()
{
	PxSceneDesc sceneDesc(mSDK->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, 0, 0);
	sceneDesc.filterShader = pDefaultFilterShader;

	PxDefaultCpuDispatcher *mCpuDispatcher = PxDefaultCpuDispatcherCreate(2);
	if (!mCpuDispatcher)
		std::cerr << "PxDefaultCpuDispatcherCreate failed!";
	sceneDesc.cpuDispatcher = mCpuDispatcher;

	mScene = mSDK->createScene(sceneDesc);
	if (!mScene)
		std::cerr << "createScene failed!";
}

PhysicEngine::~PhysicEngine()
{
	PxCloseExtensions();
	mScene->release();
	mSDK->release();
}
