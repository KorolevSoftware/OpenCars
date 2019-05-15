#pragma once
#include <d3d11.h>
#include <assert.h>


enum BufferType 
{
	Vertex = D3D11_BIND_VERTEX_BUFFER, Constant = D3D11_BIND_CONSTANT_BUFFER, Index = D3D11_BIND_INDEX_BUFFER
};

class BufferDirectX
{
private:
	ID3D11Buffer *D3DBuffer;
	ID3D11Device *device;
	BufferType type;
	unsigned int byteStride;

public:
	BufferDirectX(ID3D11Device *device);
	void create(BufferType type, int size, void * data, unsigned int byteStride = 0);
	void set(ID3D11DeviceContext *deviceContext);
	void update(ID3D11DeviceContext * deviceContext, void *data);
	~BufferDirectX();
};

