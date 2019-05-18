#pragma once
#include <d3d11.h>
#include <string>
#include "lodepng.h"
#include <vector>
#include <iostream>

class TextureDirectX
{
private:
	ID3D11ShaderResourceView *srv;
	ID3D11Device *device;

public:
	TextureDirectX(ID3D11Device *device);
	void set(ID3D11DeviceContext *deviceContext);
	void loadFormFile(const std::string &file);
	~TextureDirectX();
};

