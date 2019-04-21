#include "QuadSprite.h"
#include <fstream>

char* LoadShaderFile(std::string File, int &size)
{
	char* FileData = nullptr;

	// open the file
	std::ifstream VertexFile(File, std::ios::in | std::ios::binary | std::ios::ate);
	// if open was successful
	if (VertexFile.is_open())
	{
		// find the length of the file
		int Length = (int)VertexFile.tellg();
		size = Length;
		FileData = new char[size];
		// collect the file data
		VertexFile.seekg(0, std::ios::beg);
		VertexFile.read(FileData, Length);
		VertexFile.close();
	}

	return FileData;
}

QuadSprite::QuadSprite()
{
}

void QuadSprite::init(ID3D11Device * device)
{
	HRESULT hr;
	int sizeVS;
	int sizePS;
	char* VSFile = LoadShaderFile("e:/Project/Develop/OpenCars/x64/Debug/simpleVertexShader.cso", sizeVS);
	char* PSFile = LoadShaderFile("e:/Project/Develop/OpenCars/x64/Debug/simplePixelShader.cso", sizePS);

	hr = device->CreateVertexShader(VSFile, sizeVS, nullptr, &vertexShader);
	hr = device->CreatePixelShader(PSFile, sizePS, nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC ied[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// create and set the input layout
	hr = device->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile, sizeVS, &vertexLayout);

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

	// create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(simpleVertex) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd = { OurVertices, 0, 0 };
	hr = device->CreateBuffer(&bd, &srd, &vertexBuffer);
	if (FAILED(hr))
		return;

	D3D11_BUFFER_DESC bdm = { 0 };
	bdm.Usage = D3D11_USAGE_DEFAULT;
	bdm.ByteWidth = sizeof(ConstantBuffer);
	bdm.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = device->CreateBuffer(&bdm, nullptr, &matrixBuffer);
}


void QuadSprite::draw(const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &VP, ID3D11DeviceContext *deviceContext)
{
	// set the primitive topology
	glm::mat4 result = glm::transpose(P*glm::translate(getModelMatrix(), glm::vec3(V[3])));

	deviceContext->UpdateSubresource(matrixBuffer, 0, nullptr, &result, 0, 0);
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);
	deviceContext->IASetInputLayout(vertexLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(float) * 7;
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	// draw 6 vertices, starting from vertex 0
	deviceContext->Draw(6, 0);
}

QuadSprite::~QuadSprite()
{

}
