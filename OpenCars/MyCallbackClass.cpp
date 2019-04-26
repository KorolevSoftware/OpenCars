#include "MyCallbackClass.h"

bool MyCallbackClass::isHit()
{
	return this->hit;
}

glm::vec3 MyCallbackClass::getIntersectPoint()
{
	assert(this->hit);
	return intersectPoint;
}

glm::vec3 MyCallbackClass::getNormalInPoint()
{
	assert(this->hit);
	return normalInPoint;
}

rp3d::decimal MyCallbackClass::notifyRaycastHit(const rp3d::RaycastInfo& info)
{
	this->hit = true;
	//std::cout << "Hit point : " << info.worldPoint.x <<" "<< info.worldPoint.y <<" " << info.worldPoint.z << std::endl;
	// Return a fraction of 1.0 to gather all hits return decimal(1.0);

	this->intersectPoint = glm::vec3(info.worldPoint.x, info.worldPoint.y, info.worldPoint.z);
	this->normalInPoint = glm::vec3(info.worldNormal.x, info.worldNormal.y, info.worldNormal.z);
	this->dist = info.hitFraction;// std::min(dist, info.hitFraction);

	return dist;
}