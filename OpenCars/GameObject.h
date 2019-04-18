#pragma once
#include <d3d11.h>
#include <glm.hpp>

class GameObject
{
protected:
	GameObject *parent;
	ID3D11InputLayout*  vertexLayout;
	ID3D11Buffer*       vertexBuffer;
	ID3D11Buffer*       matrixBuffer;
	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 scale;

public:
	GameObject();
	virtual glm::mat4 getModelMatrix() = 0;
	virtual void setLocation(glm::vec3 &location) = 0;
	virtual void setRotation(glm::vec3 &rotation) = 0;
	virtual void setScale(glm::vec3 &scale) = 0;
	virtual void init(ID3D11Device *device) = 0;
	virtual void draw(glm::mat4 &VP, ID3D11DeviceContext *deviceContext) = 0;
	virtual ~GameObject();
};