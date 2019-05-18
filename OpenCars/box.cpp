#include "box.h"

Box::Box() { extent = glm::vec3(1.0f); color = glm::vec3(0, 0, 1); }

void Box::init(ID3D11Device *device)
{

	int indexes[] = {
		0, 2, 1,
		2, 6, 3,
		6, 4, 7,
		4, 0, 5,
		2, 0, 6,
		7, 5, 3,
		2, 3, 1,
		6, 7, 3,
		4, 5, 7,
		0, 1, 5,
		0, 4, 6,
		5, 1, 3
	};

	simpleVertex OurVertices[] = {
		{ glm::vec3(-extent.x , -extent.y,  extent.z), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-extent.x ,  extent.y,  extent.z), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-extent.x , -extent.y, -extent.z), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(-extent.x ,  extent.y, -extent.z), glm::vec2(0.0f, 1.0f) },

		{ glm::vec3(extent.x, -extent.y,  extent.z),  glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(extent.x,  extent.y,   extent.z), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(extent.x, -extent.y, -extent.z), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(extent.x,  extent.y,  -extent.z), glm::vec2(0.0f, 1.0f) }
	};


	vertex = new BufferDirectX(device);
	vertex->create(BufferType::Vertex, sizeof(OurVertices), OurVertices, sizeof(simpleVertex));

	index = new BufferDirectX(device);
	index->create(BufferType::Index, sizeof(indexes), indexes);

	matrix = new BufferDirectX(device);
	matrix->create(BufferType::Constant, sizeof(ConstantBuffer), nullptr);
}

void Box::setColor(const glm::vec3 & color)
{
	this->color = color;
}

void Box::setShader(ShaderDirectX * shader)
{
	this->shader = shader;
}

void Box::draw(const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &VP, ID3D11DeviceContext *deviceContext)
{
	glm::mat4 result = glm::transpose(VP*getModelMatrix());
	matrix->update(deviceContext, &result);
	shader->set(deviceContext);
	vertex->set(deviceContext);
	index->set(deviceContext);
	matrix->set(deviceContext);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->DrawIndexed(36, 0, 0);
}

Box::~Box() {}