#include "GameObject.h"

GameObject::GameObject()
{
	this->location = glm::vec3(0, 0, 0);
	this->rotation = glm::vec3(0, 0, 0);

	parent = nullptr;
	vertexLayout = nullptr;
	vertexBuffer = nullptr;
	matrixBuffer = nullptr;
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
	const float xEuler = std::atan2(matrix[1].z, matrix[2].z);
	const float cosYangle = glm::sqrt(glm::pow(matrix[0].x, 2) + glm::pow(matrix[0].y, 2));
	const float yEuler = std::atan2(-matrix[0].z, cosYangle);
	const float sinXangle = glm::sin(xEuler);
	const float cosXangle = glm::cos(xEuler);
	const float zEuler = std::atan2(
		sinXangle * matrix[2].x - cosXangle * matrix[1].x,
		cosXangle * matrix[1].y - sinXangle * matrix[2].y);
	setRotation(glm::vec3(xEuler, yEuler, zEuler));
	float a[3] = { matrix[3].x, matrix[3].y, matrix[3].z };
	setLocation(glm::vec3(matrix[3].x, matrix[3].y, matrix[3].z));
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

	if (vertexLayout)
		vertexLayout->Release();

	if (vertexBuffer)
		vertexBuffer->Release();

	if (matrixBuffer)
		matrixBuffer->Release();
}
