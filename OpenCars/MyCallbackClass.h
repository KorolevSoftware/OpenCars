#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <reactphysics3d.h>
#include <glm/glm.hpp>
#include <assert.h>

class MyCallbackClass:public rp3d::RaycastCallback
{
private:
	glm::vec3 normalInPoint;
	glm::vec3 intersectPoint;
	bool hit = false;
public:
	bool isHit();
	glm::vec3 getIntersectPoint();
	glm::vec3 getNormalInPoint();
	float dist = 10000;
	virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info);
};

