#include "BufferDirectX.h"

BufferDirectX::BufferDirectX(ID3D11Device *device)
{
	this->device = device;
	byteStride = 0;
}

void BufferDirectX::create(BufferType type, int size, void *data, unsigned int byteStride)
{
	this->type = type;
	this->byteStride = byteStride;

	HRESULT hr;
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = size;
	bd.BindFlags = type;

	D3D11_SUBRESOURCE_DATA srd = { data, 0, 0 };
	if (data == nullptr)
		hr = device->CreateBuffer(&bd, nullptr, &D3DBuffer);
	else
		hr = device->CreateBuffer(&bd, &srd, &D3DBuffer);

	assert(!FAILED(hr));
}

void BufferDirectX::set(ID3D11DeviceContext * deviceContext)
{
	UINT offset = 0;

	if(type == BufferType::Constant)
		deviceContext->VSSetConstantBuffers(0, 1, &D3DBuffer);

	if (type == BufferType::Index)
		deviceContext->IASetIndexBuffer(D3DBuffer, DXGI_FORMAT_R32_UINT, 0);

	if (type == BufferType::Vertex)
		deviceContext->IASetVertexBuffers(0, 1, &D3DBuffer, &byteStride, &offset);
}

void BufferDirectX::update(ID3D11DeviceContext * deviceContext, void *data)
{
	deviceContext->UpdateSubresource(D3DBuffer, 0, nullptr, data, 0, 0);
}

BufferDirectX::~BufferDirectX()
{
	if (D3DBuffer)
		D3DBuffer->Release();
}
