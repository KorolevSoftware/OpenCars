#pragma once
#include <d3d11.h>
#include <glm/gtx/euler_angles.hpp>	

struct ConstantBuffer
{
	glm::mat4 MVP; // Матрица мира
};

struct simpleVertex
{
	glm::vec3 pos;
	glm::vec4 col;
};

class GameObject
{
protected:
	GameObject *parent;
	glm::vec3 location;
	glm::vec3 rotation;
	glm::vec3 scale;

public:
	GameObject();
	virtual void setLocation(const glm::vec3 &location);
	virtual void setRotation(const glm::vec3 &rotation);
	virtual void setScale(const glm::vec3 &scale);
	virtual void setModelMatrix(const glm::mat4 &matrix);
	virtual void setParent(GameObject *parent);
	virtual GameObject* getParent();
	virtual glm::mat4 getModelMatrix();
	virtual glm::vec3 getLocation();
	virtual glm::vec3 getRotation();
	virtual glm::vec3 getScale();
	virtual void init(ID3D11Device *device) = 0;
	virtual void draw(const glm::mat4 &P, const glm::mat4 &V,const glm::mat4 &VP, ID3D11DeviceContext *deviceContext) = 0;
	virtual ~GameObject();
};