#include "TriangleMeshObject.h"

TriangleMeshObject::TriangleMeshObject(std::vector<float> &rawVertex, std::vector<float> rawNormal, std::vector<float> rawUV)
{
	this->rawVertex = rawVertex;
	this->rawNormal = rawNormal;
	this->rawUV = rawUV;

	triangleCount = rawVertex.size() / 3;

}

void TriangleMeshObject::init(ID3D11Device * device)
{
	simpleVertex *outVertices = new simpleVertex[triangleCount];
	simpleVertex *arrVertex = new simpleVertex[triangleCount];

	for (int i = 0; i < triangleCount; i++) 
	{
		int indexVertex = i * 3;
		int indexUV = i * 2;
		arrVertex[i] = { glm::vec3(rawVertex[indexVertex], rawVertex[indexVertex + 1], rawVertex[indexVertex + 2]), glm::vec2(rawUV[indexUV], rawUV[indexUV + 1]) };
	}

	for (int i = 0; i < triangleCount; i += 3)
	{
		outVertices[i] = arrVertex[i];
		outVertices[i+1] = arrVertex[i+2];
		outVertices[i+2] = arrVertex[i+1];
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

void TriangleMeshObject::setShader(ShaderDirectX *shader)
{
	this->shader = shader;
}


void TriangleMeshObject::draw(const glm::mat4 & P, const glm::mat4 & V, const glm::mat4 & VP, ID3D11DeviceContext * deviceContext)
{
	glm::mat4 result = glm::transpose(VP*getModelMatrix());
	matrix->update(deviceContext, &result);
	shader->set(deviceContext);
	vertex->set(deviceContext);
	matrix->set(deviceContext);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext->Draw(triangleCount, 0);
}

TriangleMeshObject::~TriangleMeshObject()
{
}
