#pragma once
#include "GameObject.h"
#include "ShaderDirectX.h"
#include "BufferDirectX.h"


class QuadSprite :
	public GameObject
{
private:
	ShaderDirectX *shader;

	BufferDirectX *vertex;
	BufferDirectX *matrix;

public:
	QuadSprite();
	void init(ID3D11Device *device) override;
	void draw(const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &VP, ID3D11DeviceContext *deviceContext) override;
	~QuadSprite();
};

