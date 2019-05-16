#pragma once
#include "GameObject.h"
#include <vector>
#include "BufferDirectX.h"
#include "ShaderDirectX.h"

class TriangleMeshObject :
	public GameObject
{
private:
	std::vector<float> vertexRaw;
	glm::vec3 color;
	BufferDirectX *vertex;
	BufferDirectX *matrix;
	ShaderDirectX *shader;

	int triangleCount;

public:
	TriangleMeshObject(std::vector<float> &vertexRaw);
	void init(ID3D11Device *device) override;
	void setColor(const glm::vec3 &color);
	void draw(const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &VP, ID3D11DeviceContext *deviceContext) override;
	~TriangleMeshObject();
};

