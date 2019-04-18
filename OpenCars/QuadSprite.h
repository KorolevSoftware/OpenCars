#pragma once
#include "GameObject.h"
#include <gtc/matrix_transform.hpp>
#include <gtx/euler_angles.hpp>

struct ConstantBuffer
{
	glm::mat4 MVP; // Матрица мира
};

struct simpleVertex
{
	glm::vec3 pos;
	glm::vec4 col;
};

class QuadSprite :
	public GameObject
{
public:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader*  pixelShader;
	QuadSprite();

	void init(ID3D11Device *device) override;
	void setLocation(glm::vec3 &location) override;
	void setRotation(glm::vec3 &rotation) override;
	void setScale(glm::vec3 &scale) override;

	glm::mat4 getModelMatrix() override;
	void draw(glm::mat4 &VP, ID3D11DeviceContext *deviceContext) override;
	~QuadSprite();
};

