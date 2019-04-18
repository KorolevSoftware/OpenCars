#include "DirectX11Engine.h"
#include <directxmath.h>
LRESULT CALLBACK WndProc(HWND hWND, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
			// ���� ������ Esc - �������
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWND, &ps);
		EndPaint(hWND, &ps);
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		
	default:
		return DefWindowProc(hWND, message, wParam, lParam);
	}
	return 0;
}

DirectX11Engine::DirectX11Engine(){}

void DirectX11Engine::createWindows(bool fullScreen, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
}

void DirectX11Engine::init()
{
	if (FAILED(initWindow()))
		return;

	initSwapChain();
	InitRenderTargetView();
}

void DirectX11Engine::render()
{
	static float t = 0.0f;
	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();

	if (timeStart == 0)
		timeStart = timeCur;

	t = (timeCur - timeStart) / 1000.0f;

	// ������ ������� ������ �����

	float ClearColor[4] = { 0.0f,0.0f, 0.0f, 1.0f }; // �������, �������, �����, �����-�����

	deviceContext->ClearRenderTargetView(renderTargetView, ClearColor);
	deviceContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	UINT stride = sizeof(float) * 7;
	UINT offset = 0;

	glm::mat4 transate = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.0, -t*10));
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 proj = glm::perspective(glm::radians(60.0f), float(windowWidth) / float(windowHeight), 0.1f, 100.0f);
	trans = glm::transpose(glm::rotate(trans, glm::radians(t*10), glm::vec3(0.0f, 1.0f, 0.0f)));

	DirectX::XMMATRIX projx = DirectX::XMMatrixPerspectiveFovLH(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	glm::mat4 MVP = proj*transate;

	for (auto i : gameObjectDraw)
		i->draw(MVP, deviceContext);


	// ��������� ������ ����� �� �����
	swapChain->Present(0, 0);

}

void DirectX11Engine::initObjects()
{
	for (auto i : gameObjectDraw)
		i->init(device);
}

HRESULT DirectX11Engine::initWindow()
{
	hInstance = GetModuleHandle(NULL);

	WNDCLASS wc;
	// ��� �������������� ����������
	wc.style = 0;
	// ������ ���������� �������
	wc.lpfnWndProc = &WndProc;
	// ��� �� ����� �������������� ��������� ������ � ��������� ���� � ��������� ������
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	// ������ (���������), �������� ����������� ���������� �������
	wc.hInstance = hInstance;
	// ��������� ����������� ������ � ����������� ������
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	// ���� ���� �� �����, ������� �� ������ "�����"
	wc.hbrBackground = nullptr;
	// ���� ��� ����
	wc.lpszMenuName = nullptr;
	// ������ �������� ������ ����
	wc.lpszClassName = _T("WindowClass1");

	// ������������ ����� ����
	ATOM result = RegisterClass(&wc);
	// ���������, ��� ����� ������� ���������������
	assert(result);

	// ����������� ���� -- ����� ���������, ����� �������� ������ � �.�.
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	RECT rect;
	// ���������� ������� (������ �����) �� ������ ������ � ��������� �������
	rect.left = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
	rect.top = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;
	rect.right = rect.left + windowWidth;
	rect.bottom = rect.top + windowHeight;
	// �������� ������� ���� � ������. ��������� �������� -- ������� ����
	AdjustWindowRect(&rect, dwStyle, FALSE);

	hWnd = CreateWindow(
		_T("WindowClass1"),
		_T("WindowName1"),
		dwStyle,
		// ����� ������� ���� ����
		rect.left, rect.top,
		// ������ ����
		rect.right - rect.left,
		rect.bottom - rect.top,
		// ������������ ����
		// HWND_DESKTOP ������������ � NULL
		HWND_DESKTOP,
		// ����
		nullptr,
		// ������ (���������), ������� ����������� ����
		hInstance,
		// �������������� ��������
		nullptr);
	// ���������, ��� ���� ������� �������
	assert(hWnd);

	ShowWindow(hWnd, SW_SHOW);
	return 0;
}

void DirectX11Engine::initSwapChain()
{
	HRESULT result;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	// ������� ��������� � �������� ���������� ����� ����
	swapChainDesc.BufferDesc.Width = windowWidth;
	swapChainDesc.BufferDesc.Height = windowHeight;
	// ����������� ���������� ������ � ������� �������� � ���� ������������� �����
	// �.�. ��� ����� ������������ ������� ������, ���������
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	// ������ ������ -- 32-������ RGBA
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// �� ������ ��������������� ��� ������
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// �� ���������� �����������
	swapChainDesc.SampleDesc.Count = 8;
	swapChainDesc.SampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_PATTERN;
	// ���������� SwapChain ��� ������
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// ���� "������" (�� ������������) �����
	swapChainDesc.BufferCount = 1;
	// ������ ���� ��� ������
	swapChainDesc.OutputWindow = hWnd;
	// ������� �����
	swapChainDesc.Windowed = TRUE;
	// ����������� ������ ���������� �� ������ ��� ������ �� �����
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// ���������� DirectX 11.0, �.�. ��� ��� ����������
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	// � Debug-������ ������� ����� ������� DirectX
#ifndef DEBUG
	UINT flags = D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT flags = 0;
#endif

	result = D3D11CreateDeviceAndSwapChain(
		// ���������� ������������ ��-���������
		nullptr,
		// ���������� ���������� ����������
		D3D_DRIVER_TYPE_HARDWARE, nullptr,
		// ��. ����
		flags,
		// ���������� ���� ������ DirectX
		&featureLevel, 1,
		// ������ SDK
		D3D11_SDK_VERSION,
		// �������� ��������� ����� ��������
		&swapChainDesc,
		// ���������, ���� �������� ���������
		&swapChain, &device, nullptr, &deviceContext);
	// ���������, ��� �������� ������ �������
	assert(SUCCEEDED(result));
}

void DirectX11Engine::InitRenderTargetView()
{
	HRESULT result;
	ID3D11Texture2D *backBuffer;

	// ����� "������" ����� �� SwapChain
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backBuffer);
	assert(SUCCEEDED(result));

	// �������������� ������ � ������ ��� ������ � ��� ���������
	result = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
	assert(SUCCEEDED(result));

	// ��������� �� ����� ������ ��� �� �����
	// ����� ��������, ��� ��� ����� ��� ���� �� ���������,
	// �.�. �� ���� �� ��� ��������� SwapChain,
	// Release() ���� ����������� ���������
	backBuffer->Release();

	// ���������� ��������� View ��� ���������
	deviceContext->OMSetRenderTargets(1, &renderTargetView, g_pDepthStencilView);

	// ������ ������� ���������
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)windowWidth;
	viewport.Height = (FLOAT)windowHeight;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	deviceContext->RSSetViewports(1, &viewport);

	D3D11_TEXTURE2D_DESC descDepth = {};     // ��������� � �����������
	descDepth.Width = windowWidth;            // ������ �
	descDepth.Height = windowHeight;    // ������ ��������
	descDepth.MipLevels = 1;            // ������� ������������
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // ������ (������ �������)
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;         // ��� - ����� ������
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	// ��� ������ ����������� ���������-�������� ������� ������ ��������

	device->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};// ��������� � �����������
	descDSV.Format = descDepth.Format;         // ������ ��� � ��������
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	// ��� ������ ����������� ���������-�������� � �������� ������� ������ ������ ������
	device->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);

}

DirectX11Engine::~DirectX11Engine()
{
	DisposeWindows();
	DisposeSwapChain();
}

void DirectX11Engine::addGemeObject(GameObject * gameObject)
{
	gameObjectDraw.push_back(gameObject);
}

void DirectX11Engine::DisposeSwapChain()
{
	deviceContext->Release();
	device->Release();
	swapChain->Release();
}

void DirectX11Engine::DisposeWindows()
{
	// ������� ����
	DestroyWindow(hWnd);
	// ������� �����
	UnregisterClass(_T("WindowClass1"), hInstance);
}