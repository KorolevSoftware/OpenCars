#include <iostream>
#include <ctime>
#include "DirectX11Engine.h"
#include "PhysicEngine.h"
#include "QuadSprite.h"
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "box.h"
#include <algorithm>
#include "TriangleMeshObject.h"
#include "RigidBodyDynamic.h"
#include "RigidBodyStatic.h"
#include "ShaderDirectX.h"



float deltaTime;


void loadModel(std::vector<float> &rawVertex, std::vector<float> &rawNormal, std::vector<float> &rawUV)
{

	std::vector<float> tempVertex;
	std::vector<float> tempNormal;
	std::vector<float> tempuv;

	auto setData = [](int startIndex, int count, std::vector<float> &sourceBuffer, std::vector<float> &forBuffer)
	{
		int vertexIndex = startIndex - 1;
		float *pVertex = &sourceBuffer[vertexIndex * count];
		forBuffer.push_back(pVertex[0]);
		forBuffer.push_back(pVertex[1]);
		if (count == 3)
			forBuffer.push_back(pVertex[2]);
	};

	std::ifstream steam("e:/Project/Develop/OpenCars/Models/tracks/TestTrack.obj");
	if (!steam.is_open())
		return;
	float x, y, z;
	int v1, t1, n1, v2, t2, n2, v3, t3, n3;
	std::string s;
	while (getline(steam, s))
	{
		if (sscanf_s(s.c_str(), "v %f %f %f", &x, &y, &z))
		{
			tempVertex.push_back(x);
			tempVertex.push_back(y);
			tempVertex.push_back(z);
		}

		if (sscanf_s(s.c_str(), "vt %f %f", &x, &y))
		{
			tempuv.push_back(x);
			tempuv.push_back(y);
		}

		if (sscanf_s(s.c_str(), "vn %f %f %f", &x, &y, &z))
		{
			tempNormal.push_back(x);
			tempNormal.push_back(y);
			tempNormal.push_back(z);
		}

		if (sscanf_s(s.c_str(), "f %i/%i/%i %i/%i/%i %i/%i/%i", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3))
		{
			setData(v1, 3, tempVertex, rawVertex);
			setData(v2, 3, tempVertex, rawVertex);
			setData(v3, 3, tempVertex, rawVertex);

			setData(t1, 2, tempuv, rawUV);
			setData(t2, 2, tempuv, rawUV);
			setData(t3, 2, tempuv, rawUV);

			setData(n1, 3, tempNormal, rawNormal);
			setData(n2, 3, tempNormal, rawNormal);
			setData(n3, 3, tempNormal, rawNormal);
		}
	}
}

int main(int argc, char* argv[])
{

	std::cout << argv[0];
	// Graphich Engine
	Camera *cam = new Camera(
		glm::vec3(0,0, 10),
		glm::vec3(0, 0.01, 0),
		glm::vec3(0, 1, 0));

	// Physic
	//---------------------Begin-----------------------------
	PhysicEngine pEngine;
	RigidBodyDynamic *carBody = pEngine.createRigidBodyDynamic();
	carBody->setLocation(glm::vec3(0, 5, 0));
	carBody->setBoxCollision(glm::vec3(2, 1, 1));
	RigidBodyStatic *trackBody = pEngine.createRigidBodyStatic();

	// Graphic
	//---------------------Begin-----------------------------
	DirectX11Engine engine;
	engine.createWindows(true, 1920, 1080);
	engine.init();
	engine.setActiveCamera(cam);
	ShaderDirectX *shader = new ShaderDirectX(engine.getDevice());


	Box *carObject = new Box(glm::vec3(2, 1, 1));
	carObject->setShader(shader);
	Box *point1 = new Box(glm::vec3(0.2, 0.2, 0.2));
	point1->setShader(shader);
	point1->setColor(glm::vec3(0, 1, 0));
	point1->setParent(carObject);

	Box *test = new Box(glm::vec3(2, 2, 2));
	test->setLocation(glm::vec3(0, 15, 0));
	test->setColor((glm::vec3(1, 0, 1)));
	test->setShader(shader);
	Box *point2 = new Box(glm::vec3(0.2, 0.2, 0.2));
	point2->setShader(shader);
	point2->setColor(glm::vec3(0, 1, 0));
	point2->setParent(carObject);

	Box *point3 = new Box(glm::vec3(0.2, 0.2, 0.2));
	point3->setShader(shader);
	point3->setColor(glm::vec3(0, 1, 0));
	point3->setParent(carObject);

	Box *point4 = new Box(glm::vec3(0.2, 0.2, 0.2));
	point4->setShader(shader);
	point4->setColor(glm::vec3(0, 1, 0));
	point4->setParent(carObject);

	point1->setLocation(glm::vec3(2.5, 0.5, 2.5));
	point2->setLocation(glm::vec3(2.5, 0.5, -2.5));
	point3->setLocation(glm::vec3(-2.5, 0.5, 2.5));
	point4->setLocation(glm::vec3(-2.5, 0.5, -2.5));

	Box *pointIntersect1 = new Box(glm::vec3(0.2, 0.2, 0.2));
	pointIntersect1->setShader(shader);
	pointIntersect1->setColor(glm::vec3(0, 1, 0));

	Box *pointIntersect2 = new Box(glm::vec3(0.2, 0.2, 0.2));
	pointIntersect2->setShader(shader);
	pointIntersect2->setColor(glm::vec3(0, 1, 0));

	Box *pointIntersect3 = new Box(glm::vec3(0.2, 0.2, 0.2));
	pointIntersect3->setShader(shader);
	pointIntersect3->setColor(glm::vec3(0, 1, 0));

	Box *pointIntersect4 = new Box(glm::vec3(0.2, 0.2, 0.2));
	pointIntersect4->setShader(shader);
	pointIntersect4->setColor(glm::vec3(0, 1, 0));


	std::vector<float> rawVertex;
	std::vector<float> rawNormal;
	std::vector<float> rawUV;

	loadModel(rawVertex, rawNormal, rawUV);

	TriangleMeshObject *trackObject = new TriangleMeshObject(rawVertex, rawNormal, rawUV);
	trackObject->setLocation(glm::vec3(0, 0, 0));
	trackObject->setShader(shader);
	trackObject->setColor(glm::vec3(1, 0.5, 1));
	trackBody->setTraiangleMesh(rawVertex);

	engine.addGemeObject(carObject);
	engine.addGemeObject(trackObject);
	engine.addGemeObject(pointIntersect4);
	engine.addGemeObject(test);
	engine.addGemeObject(pointIntersect3);
	engine.addGemeObject(pointIntersect2);
	engine.addGemeObject(pointIntersect1);
	engine.addGemeObject(point1);
	engine.addGemeObject(point2);
	engine.addGemeObject(point3);
	engine.addGemeObject(point4);
	engine.initObjects();

	MSG msg{ 0 };
	// здесь должен быть фрагмент кода, время выполнения которого нужно измерить

	while (WM_QUIT != msg.message)
	{

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		unsigned int start = clock(); // конечное время

		//Sleep(16);
		carObject->setModelMatrix(carBody->getWorldMatrix());
		trackObject->setModelMatrix(trackBody->getWorldMatrix());
		Box *boxArr[] = { point1, point2, point3, point4 };
		Box *boxIntersectArr[] = { pointIntersect1, pointIntersect2, pointIntersect3, pointIntersect4 };

		std::cout << deltaTime << std::endl;

		//carBody->addForce(glm::vec3(0, -100, 0));

		bool globalStatus = false;
		for (int i = 0; i < 4; i++)
		{
			glm::vec3 pos = boxArr[i]->getModelMatrix()[3];

			float maxDistance = 3.0;           // [in] Raycast max distance
			PxRaycastBuffer hit;                 // [out] Raycast results

			bool status = pEngine.raycast(pos, -glm::normalize(carObject->getUp()), maxDistance, hit);
			if (status)
			{
				carBody->addForceAtPos(glm::vec3(0, 1, 0) * 100.0f * (1.0f - (hit.block.distance / 3.0f)), pos);
				boxIntersectArr[i]->setLocation(glm::vec3(hit.block.position.x, hit.block.position.y, hit.block.position.z));
				globalStatus = true;
			}
			else
			{
				if (carObject->getModelMatrix()[3].y > pos.y)
					carBody->addForceAtPos(glm::vec3(0, 100, 0), pos);
				else
					carBody->addForceAtPos(glm::vec3(0, -100, 0), pos);
			}
		}

		if (globalStatus)
		{
			if (GetAsyncKeyState(VK_UP))
				carBody->addForce(carObject->getForward() * -1000.0f);

			if (GetAsyncKeyState(VK_DOWN))
				carBody->addForce(carObject->getForward() * 1000.0f);

			if (GetAsyncKeyState(VK_RIGHT))
				carBody->addRelativeTorque(glm::vec3(0, -30, 0));

			if (GetAsyncKeyState(VK_LEFT))
				carBody->addRelativeTorque(glm::vec3(0, 30, 0));
		}

		carBody->setLinearVelocity(carBody->getLinearVelocity() * (1.0f - deltaTime * 3.0f));
		carBody->setAngularVelocity(carBody->getAngularVelocity() * (1.0f - deltaTime * 10.0f));

		glm::vec3 carPos = carObject->getModelMatrix()[3];
		glm::vec3 camOffset = carObject->getForward() * 10.0f;
		cam->setTarget(carPos);
		cam->setLocation(glm::vec3(carPos.x + camOffset.x, 7, carPos.z + camOffset.z));

		pEngine.simulate(0.001);
		engine.render();
		unsigned int end = clock();
		deltaTime = (float)(end - start) / 1000.0f;

	}
	std::cout << "\nLooping ended" << std::endl;
	std::cout << "Closed" << std::endl;
	return 0;
}