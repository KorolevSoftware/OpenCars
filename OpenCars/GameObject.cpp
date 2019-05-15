#include "GameObject.h"

GameObject::GameObject()
{
	this->location = glm::vec3(0, 0, 0);
	this->rotation = glm::vec3(0, 0, 0);

	parent = nullptr;
}

void GameObject::setLocation(const glm::vec3 & location)
{
	this->location = location;
}

void GameObject::setRotation(const glm::vec3 & rotation)
{
	this->rotation = rotation;
}

void GameObject::setScale(const glm::vec3 & scale)
{
	this->scale = scale;
}

void GameObject::setModelMatrix(const glm::mat4 & matrix)
{
	float xEuler, yEuler, zEuler;
	glm::extractEulerAngleXYZ(matrix, xEuler, yEuler, zEuler);
	setRotation(glm::vec3(xEuler, yEuler, zEuler));
	setLocation(matrix[3]);
}

void GameObject::setParent(GameObject * parent)
{
	this->parent = parent;
}

GameObject * GameObject::getParent()
{
	return this->parent;
}

glm::vec3 GameObject::getLocation()
{
		return this->location;
}

glm::vec3 GameObject::getRotation()
{
	return this->rotation;
}

glm::vec3 GameObject::getScale()
{
	return this->scale;
}

glm::mat4 GameObject::getModelMatrix()
{
	glm::mat4 m = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
	m[3] = glm::vec4(this->location, 1.0f);
	if(parent == nullptr)
		return m;
	return parent->getModelMatrix()*m;
}

GameObject::~GameObject()
{
	if (parent)
		delete parent;
}
