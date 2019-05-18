#pragma once
#include "GameObject.h"
#include <vector>
#include "BufferDirectX.h"
#include "ShaderDirectX.h"
#include "TextureDirectX.h"

class TriangleMeshObject :
	public GameObject
{
private:
	std::vector<float> rawVertex;
	std::vector<float> rawNormal;
	std::vector<float> rawUV;

	glm::vec3 color;
	BufferDirectX *vertex;
	BufferDirectX *matrix;
	ShaderDirectX *shader;
	TextureDirectX *texture;
	int triangleCount;

public:
	TriangleMeshObject(std::vector<float> &rawVertex, std::vector<float> rawNormal, std::vector<float> rawUV);
	void init(ID3D11Device *device) override;
	void setColor(const glm::vec3 &color);
	void setShader(ShaderDirectX *shader);
	void draw(const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &VP, ID3D11DeviceContext *deviceContext) override;
	~TriangleMeshObject();
};

