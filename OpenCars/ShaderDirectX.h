#pragma once
#include <d3d11.h>
#include <string>
#include <fstream>
#include "TextureDirectX.h"

class ShaderDirectX
{
private:
	ID3D11Device *device;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader*  pixelShader;
	ID3D11InputLayout*  vertexLayout;
	ID3D11SamplerState* samplerLinear;
	TextureDirectX *texture;

public:
	char *LoadShaderFile(std::string File, int &size);
	ShaderDirectX(ID3D11Device *device);
	void loadTexture(std::string path);
	void set(ID3D11DeviceContext *deviceContext);
	~ShaderDirectX();
};

