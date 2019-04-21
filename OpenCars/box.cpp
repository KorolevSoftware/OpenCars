#include "box.h"

char* Box::LoadShaderFile(std::string File, int &size)
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

Box::Box() { extent = glm::vec3(1.0f); color = glm::vec3(0, 0, 1); }

void Box::init(ID3D11Device * device)
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
		5, 1, 3 };

	simpleVertex OurVertices[] = {
		{ glm::vec3(-extent.x , -extent.y,  extent.z), glm::vec4(color, 1.0f) },
		{ glm::vec3(-extent.x ,  extent.y,  extent.z), glm::vec4(color, 1.0f) },
		{ glm::vec3(-extent.x , -extent.y, -extent.z), glm::vec4(color, 1.0f) },
		{ glm::vec3(-extent.x ,  extent.y, -extent.z), glm::vec4(color, 1.0f) },

		{ glm::vec3(extent.x, -extent.y,  extent.z),  glm::vec4(color, 1.0f) },
		{ glm::vec3(extent.x,  extent.y,   extent.z), glm::vec4(color, 1.0f) },
		{ glm::vec3(extent.x, -extent.y, -extent.z), glm::vec4(color, 1.0f) },
		{ glm::vec3(extent.x,  extent.y,  -extent.z), glm::vec4(color, 1.0f) }
	};


	// create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(OurVertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd = { OurVertices, 0, 0 };
	hr = device->CreateBuffer(&bd, &srd, &vertexBuffer);
	if (FAILED(hr))
		return;

	D3D11_BUFFER_DESC bdIndex = { 0 };
	bdIndex.ByteWidth = sizeof(indexes);
	bdIndex.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA srdIndex = { indexes, 0, 0 };
	hr = device->CreateBuffer(&bdIndex, &srdIndex, &indexBuffer);
	if (FAILED(hr))
		return;

	D3D11_BUFFER_DESC bdm = { 0 };
	bdm.Usage = D3D11_USAGE_DEFAULT;
	bdm.ByteWidth = sizeof(ConstantBuffer);
	bdm.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	hr = device->CreateBuffer(&bdm, nullptr, &matrixBuffer);
}

void Box::setColor(const glm::vec3 & color)
{
	this->color = color;
}

void Box::draw(const glm::mat4 &P, const glm::mat4 &V, const glm::mat4 &VP, ID3D11DeviceContext * deviceContext)
{
	glm::mat4 result = glm::transpose(VP*getModelMatrix());

	deviceContext->UpdateSubresource(matrixBuffer, 0, nullptr, &result, 0, 0);
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);
	deviceContext->IASetInputLayout(vertexLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT stride = sizeof(simpleVertex);
	UINT offset = 0;
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	deviceContext->DrawIndexed(36,0, 0);
}

Box::~Box() {}