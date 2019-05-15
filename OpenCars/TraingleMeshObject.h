#pragma once
#include "GameObject.h"


class TraingleMeshObject :
	public GameObject
{
public:
	TraingleMeshObject();
	void init(ID3D11Device *device) override;
	void draw(const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &VP, ID3D11DeviceContext *deviceContext) override;
	~TraingleMeshObject();
};

