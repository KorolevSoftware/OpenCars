#include "QuadSprite.h"


QuadSprite::QuadSprite()
{
}

void QuadSprite::init(ID3D11Device * device)
{
	shader = new ShaderDirectX(device);
	// create a triangle out of vertices
	simpleVertex OurVertices[]
	{
		{ glm::vec3(1.0f, 1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ glm::vec3(1.0f, -1.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },

		{ glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ glm::vec3(1.0f, 1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec4(0.0f, 1.0f, 1.0f, 1.0f) },
	};


	vertex = new BufferDirectX(device);
	vertex->create(BufferType::Vertex, sizeof(OurVertices), OurVertices, sizeof(simpleVertex));

	matrix = new BufferDirectX(device);
	matrix->create(BufferType::Constant, sizeof(ConstantBuffer), nullptr);
}


void QuadSprite::draw(const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &VP, ID3D11DeviceContext *deviceContext)
{
	glm::mat4 result = glm::transpose(P*glm::translate(getModelMatrix(), glm::vec3(V[3])));
	matrix->update(deviceContext, &result);
	shader->set(deviceContext);
	matrix->set(deviceContext);
	vertex->set(deviceContext);
	
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// draw 6 vertices, starting from vertex 0
	deviceContext->Draw(6, 0);
}

QuadSprite::~QuadSprite()
{

}
