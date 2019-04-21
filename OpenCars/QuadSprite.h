#pragma once
#include "GameObject.h"



class QuadSprite :
	public GameObject
{
public:
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader*  pixelShader;
	QuadSprite();

	void init(ID3D11Device *device) override;
	void draw(const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &VP, ID3D11DeviceContext *deviceContext) override;
	~QuadSprite();
};

