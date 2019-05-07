#include "stdafx.h"
#include "WindowSystem.h"
#include "Utilities.h"
#include <string>

WindowSystem::WindowSystem() {}

WindowSystem::~WindowSystem()
{
	Utilities::get().SafeRelease(&m_pRenderTarget);
	Utilities::get().SafeRelease(&m_pD2DFactory);
}

HRESULT WindowSystem::Init()
{
	//Add initial message listeners
	m_mainWindowMessageListeners[WM_DISPLAYCHANGE] = [](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) { InvalidateRect(hwnd, NULL, FALSE); };
	m_mainWindowMessageListeners[WM_DESTROY] = [](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) { PostQuitMessage(0); };
	m_mainWindowMessageListeners[WM_PAINT] = [](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{ 
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hdc, &ps.rcPaint, hBrush);

		EndPaint(hwnd, &ps);
		DeleteObject(hBrush);
	};
	m_mainWindowMessageListeners[WM_SIZE] = [](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{ 
		UINT width = LOWORD(lParam); 
		UINT height = HIWORD(lParam);

		WindowSystem* wndSys = &WindowSystem::get();
		wndSys->ResizeRenderWindow(width, height);
	};

	m_renderWindowMessageListeners[WM_DISPLAYCHANGE] = [](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) { InvalidateRect(hwnd, NULL, FALSE); };
	m_renderWindowMessageListeners[WM_DESTROY] = [](HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) { PostQuitMessage(0); };

	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE::D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);

	if (SUCCEEDED(hr))
	{
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WindowSystem::MainWndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = NULL;
		wcex.lpszMenuName = NULL;
		wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
		wcex.lpszClassName = L"AtS Project";

		RegisterClassEx(&wcex);
		
		SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
		
		m_mainHwnd = CreateWindow(
			L"AtS Project",
			L"Ats Project",
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			CW_DEFAULT,
			CW_DEFAULT,
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
		);

		/*m_mainHwnd = CreateWindow(
			L"AtS Project",
			L"Ats Project",
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			200,
			200,
			NULL,
			NULL,
			HINST_THISCOMPONENT,
			this
		);*/

		hr = m_mainHwnd ? S_OK : E_FAIL;
		hr = CreateRenderWindow();

		if (SUCCEEDED(hr))
		{
			hr = CreateDeviceResource();
		}
		if (SUCCEEDED(hr))
		{
			int width = GetSystemMetrics(SM_CXSCREEN);
			int height = GetSystemMetrics(SM_CYSCREEN);
			SetWindowPos(m_mainHwnd, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);
			//SetWindowPos(m_mainHwnd, HWND_TOPMOST, 0, 0, 200, 200, SWP_SHOWWINDOW);

			//ViewSystem::get().FullScreen();

			ShowWindow(m_mainHwnd, SW_SHOWNORMAL);
			ShowWindow(m_renderHwnd, SW_SHOWNORMAL);
		}
	}

	return hr;
}

LRESULT CALLBACK WindowSystem::MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		WindowSystem* wndSys = (WindowSystem*)pcs->lpCreateParams;

		SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(wndSys));

		result = 1;
	}
	else
	{
		WindowSystem* wndSys = reinterpret_cast<WindowSystem*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
		bool wasHandled = false;

		if (wndSys)
		{
			auto listener = wndSys->m_mainWindowMessageListeners.find(message);
			if (listener != wndSys->m_mainWindowMessageListeners.end())
			{
				listener->second(hwnd, message, wParam, lParam);
				wasHandled = true;
				result = 0;
			}
		}
		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}

LRESULT CALLBACK WindowSystem::RenderWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		WindowSystem* wndSys = (WindowSystem*)pcs->lpCreateParams;

		SetWindowLongPtrW(hwnd, GWLP_USERDATA, PtrToUlong(wndSys));

		result = 1;
	}
	else
	{
		WindowSystem* wndSys = reinterpret_cast<WindowSystem*>(static_cast<LONG_PTR>(GetWindowLongPtrW(hwnd, GWLP_USERDATA)));
		bool wasHandled = false;

		if (wndSys)
		{
			auto listener = wndSys->m_renderWindowMessageListeners.find(message);
			if (listener != wndSys->m_renderWindowMessageListeners.end())
			{
				listener->second(hwnd, message, wParam, lParam);
				wasHandled = true;
				result = 0;
			}
		}
		if (!wasHandled)
		{
			result = DefWindowProc(hwnd, message, wParam, lParam);
		}
	}

	return result;
}

HRESULT WindowSystem::CreateRenderWindow()
{

	WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowSystem::RenderWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(LONG_PTR);
	wcex.hInstance = HINST_THISCOMPONENT;
	wcex.hbrBackground = NULL;
	wcex.lpszMenuName = NULL;
	wcex.hCursor = LoadCursor(NULL, IDI_APPLICATION);
	wcex.lpszClassName = L"RenderWindow";

	RegisterClassEx(&wcex);

	RECT rc;
	GetClientRect(m_mainHwnd, &rc);
	
	m_renderHwnd = CreateWindow(
		L"RenderWindow",
		L"RenderWindow",
		WS_CHILD | WS_VISIBLE,
		0,
		0,
		rc.right - rc.left,
		rc.bottom - rc.top,
		m_mainHwnd,
		NULL,
		HINST_THISCOMPONENT,
		this
	);

	/*m_renderHwnd = CreateWindow(
		L"RenderWindow",
		L"RenderWindow",
		WS_CHILD | WS_VISIBLE,
		0,
		0,
		200,
		200,
		m_mainHwnd,
		NULL,
		HINST_THISCOMPONENT,
		this
	);*/

	return m_renderHwnd ? S_OK : E_FAIL;
}

void WindowSystem::ResizeRenderWindow(UINT width, UINT height)
{
	UINT msWidth = (UINT)(width / 16) * 16;
	UINT mnHeight = (UINT)(height / 9) * 9;

	std::wstring str;

	float finalWidth, finalHeight;

	float ratio = 16.0f / 9.0f;

	for (finalWidth = msWidth; finalWidth > 16; finalWidth -= 16)
	{
		finalHeight = finalWidth / ratio;

		if (finalHeight <= height)
		{
			float wndPosX = ((float)width - finalWidth) / 2.0f;
			float wndPosY = ((float)height - finalHeight) / 2.0f;

			SetWindowPos(m_renderHwnd, HWND_TOP, wndPosX, wndPosY, finalWidth, finalHeight, SWP_FRAMECHANGED);
			m_pRenderTarget->Resize(D2D1::SizeU(finalWidth, finalHeight));
			InvalidateRect(m_mainHwnd, NULL, TRUE);

			break;
			return;
		}
	}
}

HRESULT WindowSystem::CreateDeviceResource()
{
	HRESULT hr = S_OK;

	if (!m_pRenderTarget)
	{
		RECT rc;
		GetClientRect(m_renderHwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top
		);
		
		D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
			96.0f,
			96.0f
		);

		hr = m_pD2DFactory->CreateHwndRenderTarget(
			properties,
			D2D1::HwndRenderTargetProperties(m_renderHwnd, size),
			&m_pRenderTarget
		);
	}

	return hr;
}

void WindowSystem::DiscardDeviceResource()
{
	Utilities::get().SafeRelease(&m_pRenderTarget);
}

HWND* WindowSystem::pMainHwnd()
{
	return &m_mainHwnd;
}

HWND* WindowSystem::pRenderHwnd()
{
	return &m_renderHwnd;
}

ID2D1HwndRenderTarget* WindowSystem::pRenderTarget()
{
	return m_pRenderTarget;
}

ID2D1Factory* WindowSystem::pFactory()
{
	return m_pD2DFactory;
}