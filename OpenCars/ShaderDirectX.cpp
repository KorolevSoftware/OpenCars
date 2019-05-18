#include "ShaderDirectX.h"



char * ShaderDirectX::LoadShaderFile(std::string File, int &size)
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

ShaderDirectX::ShaderDirectX(ID3D11Device *device)
{
	this->device = device;
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
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// create and set the input layout
	hr = device->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile, sizeVS, &vertexLayout);


	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&sampDesc, &samplerLinear);

	texture = new TextureDirectX(device);
	texture->loadFormFile("e:/Project/Real Time Render/Real_Time_Render_v2/textur/New_Graph_basecolor.png");
}

void ShaderDirectX::set(ID3D11DeviceContext *deviceContext)
{
	texture->set(deviceContext);
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
	deviceContext->IASetInputLayout(vertexLayout);
	deviceContext->PSSetSamplers(0, 1, &samplerLinear);
}


ShaderDirectX::~ShaderDirectX()
{
}
