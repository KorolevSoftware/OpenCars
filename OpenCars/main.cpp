#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <reactphysics3d.h>
#include <mathematics/Vector3.h>
#include "MyCallbackClass.h"
#include "DirectX11Engine.h"
#include <DirectXMath.h>
#include <glm/gtc/type_ptr.hpp>
#include "QuadSprite.h"
#include "Camera.h"
#include "box.h"

using namespace reactphysics3d;

enum Category {
	CATEGORY1 = 0x0001,
	CATEGORY2 = 0x0002,
	CATEGORY3 = 0x0004
};

int main(int argc, char* argv[])
{
	std::cout<< argv[0];
	// Graphich Engine
	Camera *cam = new Camera(
		glm::vec3(0, 10, 10),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));

	DirectX11Engine engine;
	engine.createWindows(true, 1920, 1080);
	engine.init();
	engine.setActiveCamera(cam);

	Box *floorG = new Box(glm::vec3(100, 0.5, 100));
	floorG->setColor(glm::vec3(1, 0, 0));

	Box *bodyG = new Box(glm::vec3(2, 1, 1));

	Box *sprite = new Box(glm::vec3(0.4, 0.4, 0.4));
	sprite->setColor(glm::vec3(1, 1, 1));

	Box *point1 = new Box(glm::vec3(0.2, 0.2, 0.2));
	point1->setColor(glm::vec3(0, 1, 0));
	point1->setParent(bodyG);

	Box *point2 = new Box(glm::vec3(0.2, 0.2, 0.2));
	point2->setColor(glm::vec3(0, 1, 0));
	point2->setParent(bodyG);

	Box *point3 = new Box(glm::vec3(0.2, 0.2, 0.2));
	point3->setColor(glm::vec3(0, 1, 0));
	point3->setParent(bodyG);

	Box *point4 = new Box(glm::vec3(0.2, 0.2, 0.2));
	point4->setColor(glm::vec3(0, 1, 0));
	point4->setParent(bodyG);

	point1->setLocation(glm::vec3( 2.5, 0.5,  2.5));
	point2->setLocation(glm::vec3( 2.5, 0.5, -2.5));
	point3->setLocation(glm::vec3(-2.5, 0.5,  2.5));
	point4->setLocation(glm::vec3(-2.5, 0.5, -2.5));

	Box *pointIntersect1 = new Box(glm::vec3(0.2, 0.2, 0.2));
	pointIntersect1->setColor(glm::vec3(0, 1, 0));
	Box *pointIntersect2 = new Box(glm::vec3(0.2, 0.2, 0.2));
	pointIntersect2->setColor(glm::vec3(0, 1, 0));
	Box *pointIntersect3 = new Box(glm::vec3(0.2, 0.2, 0.2));
	pointIntersect3->setColor(glm::vec3(0, 1, 0));
	Box *pointIntersect4 = new Box(glm::vec3(0.2, 0.2, 0.2));
	pointIntersect4->setColor(glm::vec3(0, 1, 0));


	Box *tramplinG = new Box(glm::vec3(3, 0.5, 30));
	tramplinG->setColor(glm::vec3(1, 1, 0));
	//sprite->setLocation(glm::vec3(0, 1, 0));
	bodyG->setLocation(glm::vec3(2, 0, 1));
	bodyG->setParent(floorG);
	engine.addGemeObject(floorG);
	engine.addGemeObject(bodyG);
	engine.addGemeObject(point1);
	engine.addGemeObject(point2);
	engine.addGemeObject(point3);
	engine.addGemeObject(point4);
	engine.addGemeObject(pointIntersect1);
	engine.addGemeObject(pointIntersect2);
	engine.addGemeObject(pointIntersect3);
	engine.addGemeObject(pointIntersect4);
	engine.addGemeObject(sprite);
	engine.addGemeObject(tramplinG);
	engine.initObjects();

	// end
	//// World
	rp3d::Vector3 gravity(0, 0, 0);
	rp3d::DynamicsWorld world(gravity);
	world.setNbIterationsVelocitySolver(150);
	world.setNbIterationsPositionSolver(80);
	world.enableSleeping(false);

	// body
	rp3d::Vector3 initPosition(0.0, 5.0, 0.0);
	rp3d::Transform transform(initPosition, rp3d::Quaternion(0,0.5,0.5,1));

	rp3d::RigidBody* body;
	body = world.createRigidBody(transform);
	body->setType(BodyType::DYNAMIC);
	//body->setLinearDamping(0.9);
	//body->setAngularDamping(0.1);
	rp3d::BoxShape shapeBody(Vector3(2, 1, 1));
	ProxyShape* proxyShapebody;
	proxyShapebody = body->addCollisionShape(&shapeBody, Transform::identity(), 100);
	proxyShapebody->setCollisionCategoryBits(CATEGORY2);
	//body->setCenterOfMassLocal(Vector3(0, -3, 0));
	

	rp3d::RigidBody* pol;
	pol = world.createRigidBody(Transform(Vector3(0, 0, 0), Quaternion::identity() /*Quaternion(-1, 0, 0, glm::radians(30.0f))*/));
	pol->setType(BodyType::KINEMATIC);
	rp3d::BoxShape shapePol(Vector3(100, 0.5, 100));
	ProxyShape* proxyShapePol;
	proxyShapePol = pol->addCollisionShape(&shapePol, pol->getTransform(), 0);
	proxyShapePol->setCollisionCategoryBits(CATEGORY1);

	rp3d::RigidBody* tramplin;
	tramplin = world.createRigidBody(Transform::identity());
	tramplin->setType(BodyType::KINEMATIC);
	rp3d::BoxShape shapetramplin(Vector3(3, 0.5, 30));
	ProxyShape* proxyShapetramplin;
	proxyShapetramplin = pol->addCollisionShape(&shapetramplin, Transform(Vector3(-10, 1.5, 0), Quaternion(0, 0, 1, glm::radians(10.0))), 0);
	proxyShapetramplin->setCollisionCategoryBits(CATEGORY1);


	MSG msg{ 0 };
	float damping = 500;
	float velosity = 0;
	float force;
	while (WM_QUIT != msg.message)
	{
		if (GetAsyncKeyState(VK_UP))
			body->applyForceToCenterOfMass(Vector3(-1000, 0, 0));
			//floorG->setLocation(glm::vec3(floorG->getLocation().x, 0, floorG->getLocation().z + 0.01));

		if (GetAsyncKeyState(VK_DOWN))
			body->applyForceToCenterOfMass(Vector3(1000, 0, 0));
			//floorG->setLocation(glm::vec3(floorG->getLocation().x, 0, floorG->getLocation().z - 0.01));

		if (GetAsyncKeyState(VK_RIGHT))
			body->applyTorque(Vector3(0, -1000, 0));
			//floorG->setLocation(glm::vec3(floorG->getLocation().x - 0.01, 0, floorG->getLocation().z));

		if (GetAsyncKeyState(VK_LEFT))
			body->applyTorque(Vector3(0, 1000, 0));
			//floorG->setLocation(glm::vec3(floorG->getLocation().x + 0.01, 0, floorG->getLocation().z));

		if (GetAsyncKeyState(VK_SPACE))
			floorG->setRotation(glm::vec3(0, floorG->getRotation().y + 0.01, 0));

		cam->setTarget(bodyG->getLocation());
		cam->setLocation(glm::vec3(bodyG->getLocation().x,10,10));
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			engine.render();

			Box *boxArr[] = { point1, point2, point3, point4 };
			Box *boxIntersectArr[] = { pointIntersect1, pointIntersect2, pointIntersect3, pointIntersect4 };

			float heightSpring = 3.0;


			for (int i = 0; i < 1; i++)
			{
			
			float dampingForce = body->getLinearVelocity().y * damping;
			glm::vec3 pos = glm::vec3(body->getTransform().getPosition().x, body->getTransform().getPosition().y, body->getTransform().getPosition().z);
				//glm::vec3 pos = boxArr[i]->getModelMatrix()[3];

				Ray ray(Vector3(pos.x, pos.y, pos.z), Vector3( pos.x, pos.y - heightSpring,  pos.z));
				glm::vec3 up = boxArr[i]->getModelMatrix() * glm::vec4(glm::vec3(0, 1, 0),0);
				
				//boxArr[i]->setLocation(glm::vec3(rpos.x, rpos.y, rpos.z));
				MyCallbackClass cal;
				world.raycast(ray, &cal, 1);

				if (i == 1)
					sprite->setLocation(pos);

				if (cal.isHit())
					boxIntersectArr[i]->setLocation(cal.getIntersectPoint());
				else
					boxIntersectArr[i]->setLocation(glm::vec3(pos.x, pos.y - heightSpring, pos.z));

				if (cal.isHit())
				{
					float springForceY =700* (heightSpring - cal.dist);
					float force = springForceY;
					body->applyForceToCenterOfMass(Vector3(0, force, 0));// *force, Vector3(pos.x, pos.y, pos.z));

				}
				else if (!cal.isHit())
				{
					force = -400;
					body->applyForceToCenterOfMass(Vector3(0, force, 0));
				}

				float factor = 100;
				Vector3 angularVelocity =  body->getAngularVelocity();
				std::cout << body->getTransform().getOrientation().y << std::endl;
				//body->applyTorque(-angularVelocity * factor);

				Vector3 linearVelocity = body->getLinearVelocity();
				//std::cout << linearVelocity.to_string() << std::endl;
				body->applyForceToCenterOfMass(-linearVelocity * factor);


			}
			world.update(1.0 / 1000);
			float *omFloor = new float[16];
			proxyShapePol->getLocalToWorldTransform().getOpenGLMatrix(omFloor);
			glm::mat4 *mFloor = reinterpret_cast<glm::mat4*>(omFloor);
			floorG->setModelMatrix(*mFloor);

			float *ombody = new float[16];
			body->getTransform().getOpenGLMatrix(ombody);
			glm::mat4 mbody = glm::make_mat4(ombody);
			bodyG->setModelMatrix(mbody);

			float *omTramplin = new float[16];
			proxyShapetramplin->getLocalToWorldTransform().getOpenGLMatrix(omTramplin);
			glm::mat4 mTramplin = glm::make_mat4(omTramplin);
			tramplinG->setModelMatrix(mTramplin);


		}
	}
	return 0;
}