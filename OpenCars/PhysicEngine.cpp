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

void PhysicEngine::simulate(float time)
{
	mScene->simulate(time);
	mScene->fetchResults(true);
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
}
