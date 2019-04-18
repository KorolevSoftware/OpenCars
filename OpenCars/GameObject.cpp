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
