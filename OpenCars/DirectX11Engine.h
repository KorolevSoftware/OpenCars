#pragma once
#include "GraphicEngine.h"
#include <fstream>
#include <assert.h>
#include <tchar.h>
#include <d3d11.h>
#include <vec3.hpp>
#include <mat4x4.hpp>
#include <Windows.h>
LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
struct ConstantBuffer
{
	glm::mat4 mWorld;              // Матрица мира
	glm::mat4 mView;        // Матрица вида
	glm::mat4 mProjection;  // Матрица проекции
};

class DirectX11Engine :
	public GraphicEngine
{
private:

	int windowWidth, windowHeight;
	HINSTANCE hInstance;
	HWND hWnd;
	IDXGISwapChain *swapChain;
	ID3D11Device *device;
	ID3D11DeviceContext *deviceContext;
	ID3D11RenderTargetView *renderTargetView;

	//template
	ID3D11Buffer* constantBuffer;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader*  pixelShader;
	ID3D11InputLayout*  vertexLayout;
	ID3D11Buffer*       vertexBuffer;


	 
public:
	DirectX11Engine();
	~DirectX11Engine();
	void createWindows(bool fullScreen, int width, int height) override;
	void DisposeWindows();
	void init() override;
	void render() override;
	void hellowTriangle() override;

private:
	HRESULT initWindow();
	void initSwapChain();
	void DisposeSwapChain();
	void InitRenderTargetView();
};

