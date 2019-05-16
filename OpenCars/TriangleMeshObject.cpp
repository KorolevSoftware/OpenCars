#include "TriangleMeshObject.h"

TriangleMeshObject::TriangleMeshObject(std::vector<float> &vertexRaw)
{
	this->vertexRaw = vertexRaw;
	triangleCount = vertexRaw.size() / 3;
}

void TriangleMeshObject::init(ID3D11Device * device)
{
	shader = new ShaderDirectX(device);
	simpleVertex *outVertices = new simpleVertex[triangleCount];

	for (int i = 0, v = 0; i < vertexRaw.size(); i += 3, v++)
	{
		outVertices[v] = { glm::vec3(vertexRaw[i], -vertexRaw[i+1] , vertexRaw[i+2]) , glm::vec4(color, 1.0f) };
	}


	vertex = new BufferDirectX(device);
	vertex->create(BufferType::Vertex, sizeof(simpleVertex)*triangleCount, outVertices, sizeof(simpleVertex));

	matrix = new BufferDirectX(device);
	matrix->create(BufferType::Constant, sizeof(ConstantBuffer), nullptr);
}

void TriangleMeshObject::setColor(const glm::vec3 & color)
{
	this->color = color;
}


void TriangleMeshObject::draw(const glm::mat4 & P, const glm::mat4 & V, const glm::mat4 & VP, ID3D11DeviceContext * deviceContext)
{
	glm::mat4 result = glm::transpose(VP*getModelMatrix());
	matrix->update(deviceContext, &result);
	shader->set(deviceContext);
	vertex->set(deviceContext);
	matrix->set(deviceContext);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->Draw(triangleCount*3, 0);
}

TriangleMeshObject::~TriangleMeshObject()
{
}
