#include "RigidBodyStatic.h"
RigidBodyStatic::RigidBodyStatic(PxRigidStatic * actor, PxPhysics *mSDK, PxCooking *mCooking)
{
	this->actor = actor;
	this->mSDK = mSDK;
	this->mCooking = mCooking;
}

void RigidBodyStatic::setTraiangleMesh(std::vector<float>& rawVertex)
{
	PxShape *shape;
	// Material
	PxMaterial *mMaterial = mSDK->createMaterial(0.0f, 0.0f, 0.1f);
	int *triangleIndex = new int[rawVertex.size() / 3];

	for (int i = 0; i < rawVertex.size() / 3; i += 3)
	{
		triangleIndex[i] = i;
		triangleIndex[i + 1] = i+1;
		triangleIndex[i + 2] = i+2;
	}


	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = rawVertex.size() / 3;
	meshDesc.points.stride = sizeof(PxVec3);
	meshDesc.points.data = &rawVertex[0];

	meshDesc.triangles.count = rawVertex.size() / 9;
	meshDesc.triangles.stride = 3 * sizeof(int);
	meshDesc.triangles.data = triangleIndex;

	bool ch = meshDesc.isValid();

	PxDefaultMemoryOutputStream writeBuffer;
	bool status = mCooking->cookTriangleMesh(meshDesc, writeBuffer);
	if (!status)
		return;

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());

	PxTriangleMesh* triangleMesh = mSDK->createTriangleMesh(readBuffer);
	PxTriangleMeshGeometry meshGeom(triangleMesh);
	shape = PxRigidActorExt::createExclusiveShape(*actor, meshGeom, *mMaterial);

	if (!shape)
		std::cerr << "create shape failed!";
}

glm::mat4 RigidBodyStatic::getWorldMatrix()
{
	PxMat44 mt(actor->getGlobalPose());
	return glm::make_mat4(mt.front());
}


RigidBodyStatic::~RigidBodyStatic()
{
}
