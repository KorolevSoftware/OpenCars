#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <reactphysics3d.h>
#include <mathematics/Vector3.h>
#include "MyCallbackClass.h"
#include "DirectX11Engine.h"
#include <DirectXMath.h>
using namespace reactphysics3d;
int main()
{

	DirectX11Engine engine;
	engine.createWindows(true, 800, 600);
	engine.init();
	engine.hellowTriangle();
	MSG msg = { 0 };

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			engine.render();
		}
	}
	return 0;

	//// World
	//rp3d::Vector3 gravity(0, 0, 0);
	//rp3d::DynamicsWorld world(gravity);
	//world.setNbIterationsVelocitySolver(15);
	//world.setNbIterationsPositionSolver(8);
	//world.enableSleeping(false);

	//// Body

	//rp3d::Vector3 initPosition(0.0, 5.0, 0.0);
	//rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
	//rp3d::Transform transform(initPosition, initOrientation);

	//rp3d::RigidBody* body;
	//body = world.createRigidBody(transform);
	//body->setType(BodyType::DYNAMIC);
	//body->setLinearDamping(0.1);
	//body->setAngularDamping(0.1);
	//body->setMass(10);
	//float lastContactDepth = 2.0;
	//
	//while (true)
	//{

	//	float hitDistance = body->getTransform().getPosition().y;
	//	if (hitDistance < 2.0) {
	//		float contactDepth = 2.0 - hitDistance;
	//		float contactSpeed = (lastContactDepth - contactDepth) / (1.0 / 60.0);
	//		float lastContactDepth = contactDepth;

	//		float springForce = contactDepth * 1;// springRate;
	//		float damperForce = contactSpeed * 1;// damperRate;

	//		float forcePerTire = (springForce + damperForce);

	//		lastContactDepth = contactDepth;
	//		body->applyForceToCenterOfMass(Vector3(0, forcePerTire, 0));
	//		//body->applyForceToCenterOfMass(Vector3(0, forcePerTire, 0));
	//		
	//		Vector3 force = body->getLinearVelocity();
	//		float length = force.length();
	//		float dragCoeff = 0.5*length + 0.3*length*length;
	//		force.normalize();
	//		force *= -dragCoeff;
	//		body->applyForceToCenterOfMass(force);
	//	
	//	}
	//	else if (hitDistance > 1.0)
	//		body->setLinearVelocity(Vector3(0, -1, 0));
	//	
	//	world.update(1.0 / 60.0);
	//	std::cout << body->getTransform().getPosition().y << std::endl;
	//}
	return 0;
}