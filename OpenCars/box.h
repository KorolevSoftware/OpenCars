#pragma once
#include "GameObject.h"
#include <fstream>

class Box :
	public GameObject
{

protected:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader*  pixelShader;
	glm::vec3 extent;
	glm::vec3 color;

public:
	Box();
	Box(const glm::vec3 &extent) :extent(extent), color(glm::vec3(0,0,1)){};
	char *LoadShaderFile(std::string File, int &size);
	void init(ID3D11Device *device) override;
	void setColor(const glm::vec3 &color);
	void draw(const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &VP, ID3D11DeviceContext *deviceContext) override;
	~Box();
};