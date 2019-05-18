#include "TextureDirectX.h"


TextureDirectX::TextureDirectX(ID3D11Device * device)
{
	this->device = device;
}

void TextureDirectX::set(ID3D11DeviceContext * deviceContext)
{
	deviceContext->PSSetShaderResources(0, 1, &srv);
}

void TextureDirectX::loadFormFile(const std::string & file)
{
	HRESULT hr;
	std::vector<byte> buffer;
	std::vector<byte> image;
	unsigned int width, height;
	lodepng::State state;

	//------Load and decode png--------------------
	unsigned int error;

	error = lodepng::load_file(buffer, file);
	if (error)
		std::cerr << lodepng_error_text(error);

	error= lodepng::decode(image, width, height, state, buffer);
	if (error)
		std::cerr << lodepng_error_text(error);

	///------------------- Create DirectX texture and create ShaderResView
	D3D11_TEXTURE2D_DESC t2dDesc = {};
	t2dDesc.Width = width;
	t2dDesc.Height = height;
	t2dDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	t2dDesc.MipLevels = t2dDesc.ArraySize = 1;
	t2dDesc.SampleDesc.Count = 1;
	t2dDesc.Usage = D3D11_USAGE_IMMUTABLE;
	t2dDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	const uint32_t s_pixel = 0xffc99aff;
	D3D11_SUBRESOURCE_DATA data = {0};
	data.pSysMem = image.data();
	data.SysMemPitch = width * 4;

	ID3D11Texture2D *texture;
	hr = device->CreateTexture2D(&t2dDesc, &data, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC rvDesc = {};
	rvDesc.Format = t2dDesc.Format;
	rvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	rvDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(texture, &rvDesc, &srv);
}

TextureDirectX::~TextureDirectX()
{
	if (srv)
		srv->Release();
}
