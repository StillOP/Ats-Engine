#pragma once
#include <map>
#include <functional>
#include <d3d11_1.h>
#include <d2d1.h>
#include <dxgi1_2.h>
#include "Math.h"



#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)& __ImageBase)
#endif // !HINST_THISCOMPONENT

class WindowSystem
{
public:
	~WindowSystem();
	
	HRESULT Init();

	static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK RenderWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	HRESULT CreateDeviceResource();
	void DiscardDeviceResource();

	template <class T>
	void AddMainWindowMessageListener(UINT msg, void (T::*function)(HWND, UINT, WPARAM, LPARAM), T* instance)
	{
		if (m_mainWindowMessageListeners.find(msg) != m_mainWindowMessageListeners.end()) { return; }

		auto func = std::bind(function, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		m_mainWindowMessageListeners.emplace(msg, func);
	}

	template <class T>
	void AddRenderWindowMessageListener(UINT msg, void (T::*function)(HWND, UINT, WPARAM, LPARAM), T* instance)
	{
		if (m_renderWindowMessageListeners.find(msg) != m_renderWindowMessageListeners.end()) { return; }

		auto func = std::bind(function, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		m_renderWindowMessageListeners.emplace(msg, func);
	}

	static WindowSystem& get()
	{
		static WindowSystem wndSys;
		return wndSys;
	}

	void ResizeRenderWindow(UINT width, UINT height);

	HWND* pMainHwnd();
	HWND* pRenderHwnd();

	ID2D1HwndRenderTarget* pRenderTarget();

	ID2D1Factory* pFactory();

private:
	WindowSystem();

	HRESULT CreateRenderWindow();

	HWND m_mainHwnd;
	HWND m_renderHwnd;

	ID2D1Factory* m_pD2DFactory;
	ID2D1HwndRenderTarget* m_pRenderTarget;

	std::map<UINT, std::function<void(HWND, UINT, WPARAM, LPARAM)>> m_mainWindowMessageListeners;
	std::map<UINT, std::function<void(HWND, UINT, WPARAM, LPARAM)>> m_renderWindowMessageListeners;
};
