#include <iostream>
#include <ctime>
#include "DirectX11Engine.h"
#include "QuadSprite.h"
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "box.h"
#include "PxPhysicsAPI.h"


using namespace physx;
PxPhysics *mSDK = nullptr;
PxCooking *mCooking = nullptr;
PxDefaultErrorCallback pDefaultErrorCallback;
PxDefaultAllocator pDefaultAllocatorCallback;
PxSimulationFilterShader pDefaultFilterShader = PxDefaultSimulationFilterShader;

PxScene *mScene;

PxRigidStatic *plane;
PxRigidStatic *tramplin;
PxRigidStatic *actorTriMesh;
PxShape *shape;
PxRigidDynamic * actor;

float mStepSize = 1.0f / 1000.0f;

void initPhysX();
void initScene();
void initActors();
void ShowTransformations();
void TransformPrint(PxRigidActor*);
void releasePhysX();

void initPhysX()
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

void initScene()
{
	PxSceneDesc sceneDesc(mSDK->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0, 0, 0);// PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.filterShader = pDefaultFilterShader;

	PxDefaultCpuDispatcher *mCpuDispatcher = PxDefaultCpuDispatcherCreate(2);
	if (!mCpuDispatcher)
		std::cerr << "PxDefaultCpuDispatcherCreate failed!";
	sceneDesc.cpuDispatcher = mCpuDispatcher;

	mScene = mSDK->createScene(sceneDesc);
	if (!mScene)
		std::cerr << "createScene failed!";
}
void initActors()
{
	// Material
	PxMaterial *mMaterial = mSDK->createMaterial(0.0f, 0.0f, 0.1f);
	if (!mMaterial)
		std::cerr << "createMaterial failed!";

	PxReal d = 0.0f;
	PxTransform pose = PxTransform(PxVec3(0.0f, d, 0.0f), PxQuat(PxHalfPi, PxVec3(0.0f, 0.0f, 1.0f)));

	// Plane
	plane = mSDK->createRigidStatic(pose);
	if (!plane)
		std::cerr << "create plane failed!";

	shape = PxRigidActorExt::createExclusiveShape(*plane, PxPlaneGeometry(), *mMaterial);
	tramplin = mSDK->createRigidStatic(PxTransform(PxVec3(-3.0f, 0, 0.0f), PxQuat(0.3, 0.0, 0, 1).getNormalized()));
	shape = PxRigidActorExt::createExclusiveShape(*tramplin, PxBoxGeometry(7, 1, 4), *mMaterial);

	if (!shape)
		std::cerr << "create shape failed!";

	mScene->addActor(*plane);
	PxReal density = 1.0f;
	PxTransform transform = PxTransform(PxVec3(0.0f, 5.0f, 0.0f), PxQuat(0.0, 0.0, 0, 1).getNormalized());
	PxVec3 dimensions(2.0f, 1.0f, 1.0f);
	PxBoxGeometry geometry(dimensions);


	actor = PxCreateDynamic(*mSDK, transform, geometry, *mMaterial, density);
	
	
	mScene->addActor(*actor);
	mScene->addActor(*tramplin);

	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = 8;
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = new PxVec3[8]
	{
		PxVec3(1.0, -1.0, -1.0),
		PxVec3(1.0, -1.0, 1.0),
		PxVec3(-1.0, -1.0, 1.0),
		PxVec3(-1.0, -1.0, -1.0),
		PxVec3(1.0, 1.0, -1.0),
		PxVec3(1.0, 1.0, 1.0),
		PxVec3(-1.0, 1.0, 1.0),
		PxVec3(-1.0, 1.0, -1.0)
	};

	meshDesc.triangles.count = 12;
	meshDesc.triangles.stride = 3 * sizeof(int);
	meshDesc.triangles.data = new int[36]
	{
		3, 0, 1,
		5, 4, 7,
		1, 0, 4,
		2, 1, 5,
		2, 6, 7,
		7, 4, 0,
		3, 1, 2,
		5, 7, 6,
		1, 4, 5,
		2, 5, 6,
		2, 7, 3,
		7, 0, 3
	};

	bool ch = meshDesc.isValid();

	PxDefaultMemoryOutputStream writeBuffer;
	bool status = mCooking->cookTriangleMesh(meshDesc, writeBuffer);
	if (!status)
		return;

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

	PxTriangleMesh* triangleMesh = mSDK->createTriangleMesh(readBuffer);
	PxTriangleMeshGeometry meshGeom(triangleMesh);
	actorTriMesh = mSDK->createRigidStatic(PxTransform(PxVec3(0.0f, 1.5f, 4.0f), PxQuat(0,0,0,1).getNormalized()));
	mScene->addActor(*actorTriMesh);
	shape = PxRigidActorExt::createExclusiveShape(*actorTriMesh, meshGeom, *mMaterial);

	// Cube
	if (!shape)
		std::cerr << "create shape failed!";
	if (!actor)
		std::cerr << "create actor failed!";
}

void PhysXLoop()
{
	mScene->simulate(0.001);
	mScene->fetchResults(true);	
}
void ShowTransformations()
{
	std::cout << "\tplane" << std::endl;
	TransformPrint(shape->getActor());

	std::cout << "\tcube" << std::endl;
	PxU32 nShapes = actor->getNbShapes();
	PxShape** shapes = new PxShape*[nShapes];
	actor->getShapes(shapes, nShapes);



	while (nShapes--)
		TransformPrint(shapes[nShapes]->getActor());

	delete[] shapes;
}

void TransformPrint(PxRigidActor *actor)
{
	PxTransform np = actor->getGlobalPose();
	PxMat44 m(actor->getGlobalPose());

	std::cout << "\t( " << m[0][0] << " ; " << m[0][1] << " ; " << m[0][2] << " )" << std::endl;

	std::cout << "\t( " << np.p.x << " ; " << np.p.y << " ; " << np.p.z << " )" << std::endl;
}
void releasePhysX()
{
	PxCloseExtensions();
	mScene->release();
	mSDK->release();
}

int main(int argc, char* argv[])
{
	std::cout << argv[0];
	// Graphich Engine
	Camera *cam = new Camera(
		glm::vec3(0, 10, 10),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));


	// Graphic
	//---------------------Begin-----------------------------
	DirectX11Engine engine;
	engine.createWindows(true, 1920, 1080);
	engine.init();
	engine.setActiveCamera(cam);


	initPhysX();
	initScene();
	initActors();


	Box *box = new Box(glm::vec3(2, 1, 1));
	Box *tramplinG = new Box(glm::vec3(7, 1, 4));
	Box *plane = new Box(glm::vec3(1000, 1, 1000));
	Box *boxIntersect = new Box(glm::vec3(0.3, 0.3, 0.3));
	Box *point1 = new Box(glm::vec3(0.2, 0.2, 0.2));
	Box *triangleMesh = new Box(glm::vec3(1, 1, 1));
	tramplinG->setColor(glm::vec3(1,0,1));
	point1->setColor(glm::vec3(0, 1, 0));
	point1->setParent(box);

	Box *point2 = new Box(glm::vec3(0.2, 0.2, 0.2));
	point2->setColor(glm::vec3(0, 1, 0));
	point2->setParent(box);

	Box *point3 = new Box(glm::vec3(0.2, 0.2, 0.2));
	point3->setColor(glm::vec3(0, 1, 0));
	point3->setParent(box);

	Box *point4 = new Box(glm::vec3(0.2, 0.2, 0.2));
	point4->setColor(glm::vec3(0, 1, 0));
	point4->setParent(box);

	point1->setLocation(glm::vec3(2.5, 0.5, 2.5));
	point2->setLocation(glm::vec3(2.5, 0.5, -2.5));
	point3->setLocation(glm::vec3(-2.5, 0.5, 2.5));
	point4->setLocation(glm::vec3(-2.5, 0.5, -2.5));

	Box *pointIntersect1 = new Box(glm::vec3(0.2, 0.2, 0.2));
	pointIntersect1->setColor(glm::vec3(0, 1, 0));
	Box *pointIntersect2 = new Box(glm::vec3(0.2, 0.2, 0.2));
	pointIntersect2->setColor(glm::vec3(0, 1, 0));
	Box *pointIntersect3 = new Box(glm::vec3(0.2, 0.2, 0.2));
	pointIntersect3->setColor(glm::vec3(0, 1, 0));
	Box *pointIntersect4 = new Box(glm::vec3(0.2, 0.2, 0.2));
	pointIntersect4->setColor(glm::vec3(0, 1, 0));


	triangleMesh->setLocation(glm::vec3(0, 1.5, 4));
	plane->setLocation(glm::vec3(0, -1, 0));
	plane->setColor(glm::vec3(1, 1, 0));
	boxIntersect->setColor(glm::vec3(0, 1, 1));
	engine.addGemeObject(triangleMesh);
	engine.addGemeObject(boxIntersect);
	engine.addGemeObject(tramplinG);
	engine.addGemeObject(box);
	engine.addGemeObject(plane);
	engine.addGemeObject(pointIntersect4);
	engine.addGemeObject(pointIntersect3);
	engine.addGemeObject(pointIntersect2);
	engine.addGemeObject(pointIntersect1);
	engine.addGemeObject(point1);
	engine.addGemeObject(point2);
	engine.addGemeObject(point3);
	engine.addGemeObject(point4);
	engine.initObjects();

	actor->setLinearDamping(0.5);
	actor->setAngularDamping(0.5);
	MSG msg{ 0 };
   // здесь должен быть фрагмент кода, время выполнения которого нужно измерить
	unsigned int old = 0; // конечное время
	while (WM_QUIT != msg.message)
	{
		unsigned int newt = clock();
		float deltaTime = (float)(newt - old)/1000.0f;
		old = newt;

		if (GetAsyncKeyState(VK_RIGHT))
			actor->addTorque(PxMat44(actor->getGlobalPose()).transform(PxVec3(0, -300, 0)));

		if (GetAsyncKeyState(VK_LEFT))
			actor->addTorque(PxMat44(actor->getGlobalPose()).transform(PxVec3(0, 300, 0) ));

		if (GetAsyncKeyState(VK_UP))
			actor->addForce(PxMat44(actor->getGlobalPose()).transform(PxVec3(-1000, 0, 0)));

		if (GetAsyncKeyState(VK_DOWN))
			actor->addForce(PxMat44(actor->getGlobalPose()).transform(PxVec3(1000, 0, 0)));

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			PxU32 nShapes = actor->getNbShapes();
			PxShape** shapes = new PxShape*[nShapes];
			actor->getShapes(shapes, nShapes);

			PxMat44 m(actor->getGlobalPose());
			glm::mat4 matrix = glm::make_mat4(m.front());
			box->setModelMatrix(matrix);

			PxMat44 mt(tramplin->getGlobalPose());
			tramplinG->setModelMatrix(glm::make_mat4(mt.front()));

			PxMat44 mtriangl(actorTriMesh->getGlobalPose());
			triangleMesh->setModelMatrix(glm::make_mat4(mtriangl.front()));

			actor->setAngularVelocity(actor->getAngularVelocity() * (1.0 - deltaTime * 10.0));
			actor->setLinearVelocity(actor->getLinearVelocity() * (1.0 - deltaTime * 3.0));

			Box *boxArr[] = { point1, point2, point3, point4 };
			Box *boxIntersectArr[] = { pointIntersect1, pointIntersect2, pointIntersect3, pointIntersect4 };

			/*std::cout
				<<" x "<< PxMat44(actor->getGlobalPose()).transform(PxVec3(0, 500, 0)).x
				<<" y "<< PxMat44(actor->getGlobalPose()).transform(PxVec3(0, 500, 0)).y
				<<" z "<< PxMat44(actor->getGlobalPose()).transform(PxVec3(0, 500, 0)).z
				<<std::endl;*/

			std::cout << deltaTime << std::endl;

			for (int i = 0; i < 4; i++)
			{
				glm::vec3 pos = boxArr[i]->getModelMatrix()[3];
		
				PxVec3 origin(pos.x, pos.y, pos.z);               // [in] Ray origin
				PxVec3 unitDir(0, -1, 0);                // [in] Normalized ray direction
				PxReal maxDistance = 3.0;           // [in] Raycast max distance
				PxRaycastBuffer hit;                 // [out] Raycast results

				// Raycast against all static & dynamic objects (no filtering)
				// The main result from this call is the closest hit, stored in the 'hit.block' structure
				bool status = mScene->raycast(origin, PxMat44(actor->getGlobalPose()).rotate(unitDir).getNormalized(), maxDistance, hit);
				if (status)
				{
					PxRigidBodyExt::addForceAtPos(*actor, PxMat44(actor->getGlobalPose()).transform(PxVec3(0, 100 * (1.0 - (hit.block.position.y / 3.0)), 0)), origin);
					boxIntersectArr[i]->setLocation(glm::vec3(hit.block.position.x, hit.block.position.y, hit.block.position.z));
				}
				else
				{
					if(actor->getGlobalPose().p.y > pos.y)
						PxRigidBodyExt::addForceAtPos(*actor, PxVec3(0, 100, 0), origin);
					else
						PxRigidBodyExt::addForceAtPos(*actor, PxVec3(0, -100, 0), origin);
				}
			}
			PhysXLoop();

			PxVec3 carPos = actor->getGlobalPose().p;
			PxVec3 camOffset = PxMat44(actor->getGlobalPose()).transform(PxVec3(10, 0, 0));
			cam->setTarget(glm::vec3(carPos.x, carPos.y, carPos.z));
			cam->setLocation(glm::vec3(camOffset.x, 7, camOffset.z));
			

			engine.render();
		}
	}
	std::cout << "\nLooping ended" << std::endl;

	releasePhysX();
	std::cout << "Closed" << std::endl;
	return 0;
}