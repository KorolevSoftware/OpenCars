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
			// Если нажали Esc - выходим
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

	// Просто очищаем задний буфер

	float ClearColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f }; // красный, зеленый, синий, альфа-канал

	deviceContext->ClearRenderTargetView(renderTargetView, ClearColor);
	deviceContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	/*glm::mat4 transate = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.0, -t*10));
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::transpose(glm::rotate(trans, glm::radians(t*10), glm::vec3(0.0f, 1.0f, 0.0f)));

	DirectX::XMMATRIX projx = DirectX::XMMatrixPerspectiveFovLH(glm::radians(60.0f), 4.0f / 3.0f, 0.1f, 100.0f);
*/
	glm::mat4 proj = glm::perspective(glm::radians(60.0f), float(windowWidth) / float(windowHeight), 0.1f, 1000.0f);

	glm::mat4 MVP = proj*activeCamera->getViewMatrix();

	for (auto i : gameObjectDraw)
		i->draw(proj, activeCamera->getViewMatrix(), MVP, deviceContext);


	// Выбросить задний буфер на экран
	swapChain->Present(0, 0);

}

void DirectX11Engine::initObjects()
{
	for (auto i : gameObjectDraw)
		i->init(device);
}

void DirectX11Engine::setActiveCamera(Camera * cam)
{
	this->activeCamera = cam;
}

HRESULT DirectX11Engine::initWindow()
{
	hInstance = GetModuleHandle(NULL);

	WNDCLASS wc;
	// Без дополнительных параметров
	wc.style = 0;
	// Задаем обработчик событий
	wc.lpfnWndProc = &WndProc;
	// Нам не нужно дополнительное выделение памяти к структуре окна и структуре класса
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	// Модуль (программа), которому принадлежит обработчик событий
	wc.hInstance = hInstance;
	// Загружаем стандартный курсор и стандартную иконку
	wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	// Цвет фона не важен, поэтому не задаем "кисть"
	wc.hbrBackground = nullptr;
	// Окно без меню
	wc.lpszMenuName = nullptr;
	// Задаем название класса окна
	wc.lpszClassName = _T("WindowClass1");

	// Регистрируем класс окна
	ATOM result = RegisterClass(&wc);
	// Проверяем, что класс успешно зарегистрирован
	assert(result);

	// Стандартное окно -- имеет заголовок, можно изменить размер и т.д.
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;
	RECT rect;
	// Клиентская область (внутри рамки) по центру экрана и заданного размера
	rect.left = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
	rect.top = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;
	rect.right = rect.left + windowWidth;
	rect.bottom = rect.top + windowHeight;
	// Вычислим область окна с рамкой. Последний параметр -- наличие меню
	AdjustWindowRect(&rect, dwStyle, FALSE);

	hWnd = CreateWindow(
		_T("WindowClass1"),
		_T("WindowName1"),
		dwStyle,
		// Левый верхний угол окна
		rect.left, rect.top,
		// Размер окна
		rect.right - rect.left,
		rect.bottom - rect.top,
		// Родительское окно
		// HWND_DESKTOP раскрывается в NULL
		HWND_DESKTOP,
		// Меню
		nullptr,
		// Модуль (программа), которой принадлежит окно
		hInstance,
		// Дополнительные свойства
		nullptr);
	// Проверяем, что окно успешно создано
	assert(hWnd);

	ShowWindow(hWnd, SW_SHOW);
	return 0;
}

void DirectX11Engine::initSwapChain()
{
	HRESULT result;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = windowWidth;
	swapChainDesc.BufferDesc.Height = windowHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hWnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	// Используем DirectX 11.0, т.к. его нам достаточно
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	// В Debug-версии создаем поток отладки DirectX
#ifndef DEBUG
	UINT flags = D3D11_CREATE_DEVICE_DEBUG;
#else
	UINT flags = 0;
#endif

	result = D3D11CreateDeviceAndSwapChain(
		// Используем видеоадаптер по-умолчанию
		nullptr,
		// Используем аппаратную реализацию
		D3D_DRIVER_TYPE_HARDWARE, nullptr,
		// См. выше
		flags,
		// Используем одну версию DirectX
		&featureLevel, 1,
		// Версия SDK
		D3D11_SDK_VERSION,
		// Передаем созданное ранее описание
		&swapChainDesc,
		// Указатели, куда записать результат
		&swapChain, &device, nullptr, &deviceContext);
	// Проверяем, что операция прошла успешно
	assert(SUCCEEDED(result));
}

void DirectX11Engine::InitRenderTargetView()
{
	HRESULT result;
	ID3D11Texture2D *backBuffer;

	// Берем "задний" буфер из SwapChain
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void **)&backBuffer);
	assert(SUCCEEDED(result));

	// Инициализируем доступ к буферу для записи и для отрисовки
	result = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
	assert(SUCCEEDED(result));

	// Указатель на буфер больше нам не нужен
	// Стоит отметить, что сам буфер при этом не удаляется,
	// т.к. на него всё ещё указывает SwapChain,
	// Release() лишь освобождает указатель
	backBuffer->Release();

	// Используем созданный View для отрисовки

	// Задаем область отрисовки

	D3D11_TEXTURE2D_DESC descDepth = {};     // Структура с параметрами
	descDepth.Width = windowWidth;            // ширина и
	descDepth.Height = windowHeight;    // высота текстуры
	descDepth.MipLevels = 1;            // уровень интерполяции
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // формат (размер пикселя)
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;         // вид - буфер глубин
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	// При помощи заполненной структуры-описания создаем объект текстуры

	result = device->CreateTexture2D(&descDepth, nullptr, &g_pDepthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};// Структура с параметрами
	descDSV.Format = descDepth.Format;         // формат как в текстуре
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	// При помощи заполненной структуры-описания и текстуры создаем объект буфера глубин
	result =  device->CreateDepthStencilView(g_pDepthStencil, &descDSV, &g_pDepthStencilView);

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)windowWidth;
	viewport.Height = (FLOAT)windowHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	deviceContext->RSSetViewports(1, &viewport);
	deviceContext->OMSetRenderTargets(1, &renderTargetView, g_pDepthStencilView);
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
	// Удаляем окно
	DestroyWindow(hWnd);
	// Удаляем класс
	UnregisterClass(_T("WindowClass1"), hInstance);
}