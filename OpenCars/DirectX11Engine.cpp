#include "DirectX11Engine.h"

LRESULT CALLBACK WndProc(HWND hWND, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
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
	

	// Просто очищаем задний буфер

	float ClearColor[4] = { 0.0f, 0.0f, 1.0f, 1.0f }; // красный, зеленый, синий, альфа-канал

	deviceContext->ClearRenderTargetView(renderTargetView, ClearColor);
	UINT stride = sizeof(float) * 3;
	UINT offset = 0;
	//deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// set the primitive topology
	deviceContext->VSSetShader(vertexShader, nullptr, 0);
	deviceContext->PSSetShader(pixelShader, nullptr, 0);
	deviceContext->IASetInputLayout(vertexLayout);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);


	// draw 3 vertices, starting from vertex 0
	deviceContext->Draw(3, 0);

	// Выбросить задний буфер на экран

	swapChain->Present(0, 0);

}

char* LoadShaderFile(std::string File, int &size)
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

void DirectX11Engine::hellowTriangle()
{
	HRESULT hr;
	int sizeVS;
	int sizePS;
	char* VSFile = LoadShaderFile("e:/Project/Develop/OpenCars/x64/Debug/simpleVertexShader.cso", sizeVS);
	char* PSFile = LoadShaderFile("e:/Project/Develop/OpenCars/x64/Debug/simplePixelShader.cso", sizePS);

	hr = device->CreateVertexShader(VSFile, sizeVS, nullptr, &vertexShader);
	hr = device->CreatePixelShader(PSFile, sizePS, nullptr, &pixelShader);

	if (FAILED(hr))
		return;



	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// create and set the input layout
	hr = device->CreateInputLayout(ied, ARRAYSIZE(ied), VSFile, sizeVS, &vertexLayout);

	// create a triangle out of vertices
	glm::vec3 OurVertices[] =
	{
		{ 0.0f, 0.5f, 0.0f },
	{ 0.45f, -0.5f, 0.0f },
	{ -0.45f, -0.5f, 0.0f },
	};

	// create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	bd.ByteWidth = sizeof(glm::vec3) * ARRAYSIZE(OurVertices);
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA srd = { OurVertices, 0, 0 };

	hr = device->CreateBuffer(&bd, &srd, &vertexBuffer);
	if (FAILED(hr))
		return ;
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

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	// Разрмер совпадает с размером клиентской части окна
	swapChainDesc.BufferDesc.Width = windowWidth;
	swapChainDesc.BufferDesc.Height = windowHeight;
	// Ограничение количества кадров в секунду задается в виде рационального числа
	// Т.к. нам нужна максимальная частота кадров, отключаем
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	// Формат вывода -- 32-битный RGBA
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// Не задаем масштабирования при выводе
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	// Не используем сглаживание
	swapChainDesc.SampleDesc.Count = 8;
	swapChainDesc.SampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_PATTERN;
	// Используем SwapChain для вывода
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// Один "задний" (не отображаемый) буфер
	swapChainDesc.BufferCount = 1;
	// Задаем окно для вывода
	swapChainDesc.OutputWindow = hWnd;
	// Оконный режим
	swapChainDesc.Windowed = TRUE;
	// Отбрасываем старую информацию из буфера при выводе на экран
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

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
	deviceContext->OMSetRenderTargets(1, &renderTargetView, NULL);

	// Задаем область отрисовки
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (FLOAT)windowWidth;
	viewport.Height = (FLOAT)windowHeight;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	deviceContext->RSSetViewports(1, &viewport);
}

DirectX11Engine::~DirectX11Engine()
{
	DisposeWindows();
	DisposeSwapChain();
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