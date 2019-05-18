#pragma once
#include "GameObject.h"
#include "BufferDirectX.h"
#include "ShaderDirectX.h"

class Box :
	public GameObject
{

protected:
	glm::vec3 extent;
	glm::vec3 color;

	ShaderDirectX *shader;

	BufferDirectX *vertex;
	BufferDirectX *index;
	BufferDirectX *matrix;

public:
	Box();
	Box(const glm::vec3 &extent) :extent(extent), color(glm::vec3(0,0,1)){};
	void init(ID3D11Device *device) override;
	void setColor(const glm::vec3 &color);
	void setShader(ShaderDirectX *shader);
	void draw(const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &VP, ID3D11DeviceContext *deviceContext) override;
	~Box();
};