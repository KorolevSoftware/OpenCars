#include "Camera.h"

glm::mat4 Camera::getViewMatrix()
{
	return glm::lookAt(location, target, up);
}

void Camera::setUp(const glm::vec3 & up)
{
	this->up = up;
}

void Camera::setTarget(const glm::vec3 & target)
{
	this->target = target;
}

void Camera::setLocation(const glm::vec3 & location)
{
	this->location = location;
}

glm::vec3 Camera::getUp()
{
	return this->up;
}

glm::vec3 Camera::getTarget()
{
	return this->target;
}

glm::vec3 Camera::getLocation()
{
	return this->location;
}
