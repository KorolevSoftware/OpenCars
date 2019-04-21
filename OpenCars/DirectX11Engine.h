#pragma once
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "GraphicEngine.h"
#include <fstream>
#include <assert.h>
#include <tchar.h>
#include <d3d11.h>
#include <glm/gtc/matrix_transform.hpp>
#include <Windows.h>
#include "GameObject.h"
#include "Camera.h"
#include <vector>


LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

class DirectX11Engine :
	public GraphicEngine
{
private:
	// Window
	HINSTANCE hInstance;
	HWND hWnd;
	int windowWidth, windowHeight;

	// DirectX
	IDXGISwapChain *swapChain;
	ID3D11Device *device;
	ID3D11DeviceContext *deviceContext;
	ID3D11RenderTargetView *renderTargetView;
	ID3D11Texture2D* g_pDepthStencil;
	ID3D11DepthStencilView* g_pDepthStencilView;
	Camera *activeCamera;
	//GameEngine
	std::vector<GameObject *> gameObjectDraw;

public:
	DirectX11Engine();
	~DirectX11Engine();
	void addGemeObject(GameObject * gameObject);
	void createWindows(bool fullScreen, int width, int height) override;
	void DisposeWindows();
	void init() override;
	void render() override;
	void initObjects() override;
	void setActiveCamera(Camera *cam);

private:
	HRESULT initWindow();
	void initSwapChain();
	void DisposeSwapChain();
	void InitRenderTargetView();
};

