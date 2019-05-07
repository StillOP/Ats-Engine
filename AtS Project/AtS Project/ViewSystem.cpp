#include "stdafx.h"
#include "ViewSystem.h"
#include "WindowSystem.h"
#include <string>

ViewSystem::ViewSystem() {}

ViewSystem::~ViewSystem() {}

void ViewSystem::Init()
{
	WindowSystem* wndSys = &WindowSystem::get();

	m_worldSize.x = 1920.0f;
	m_worldSize.y = 1080.0f;

	RECT rect;
	GetClientRect(*wndSys->pRenderHwnd(), &rect);
	
	m_scale.x = ((float)rect.right - (float)rect.left) / m_worldSize.x;
	m_scale.y = ((float)rect.bottom - (float)rect.top) / m_worldSize.y;

	m_scaleOrigin = { 0.0f, 0.0f };

	m_translation = { 0.0f, 0.0f };

	m_focused = true;

	m_displayMode = DISPLAY_MODE::WINDOWED;
}

void ViewSystem::Update(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_SIZE)
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);

		m_scale.x = (float)width / m_worldSize.x;
		m_scale.y = (float)height / m_worldSize.y;
	}
	else if (message == WM_SETFOCUS)
	{
		m_focused = true;
	}
	else if (message == WM_KILLFOCUS)
	{
		m_focused = false;
	}
}

void ViewSystem::ChangeDisplayMode(DISPLAY_MODE mode)
{
	if (mode == DISPLAY_MODE::WINDOWED || mode == DISPLAY_MODE::WINDOWED_FULLSCREEN)
	{
		HWND* pHwnd = WindowSystem::get().pMainHwnd();

		if (m_displayMode == DISPLAY_MODE::WINDOWED) { GetWindowRect(*pHwnd, &m_windowRect); }

		int width, height;

		width = m_displayMode == DISPLAY_MODE::WINDOWED_FULLSCREEN ? m_windowRect.right - m_windowRect.left : GetSystemMetrics(SM_CXSCREEN);
		height = m_displayMode == DISPLAY_MODE::WINDOWED_FULLSCREEN ? m_windowRect.bottom - m_windowRect.top : GetSystemMetrics(SM_CYSCREEN);

		LONG style = m_displayMode == DISPLAY_MODE::WINDOWED_FULLSCREEN ? WS_OVERLAPPEDWINDOW : WS_POPUP | WS_VISIBLE;

		SetWindowLongPtr(*pHwnd, GWL_STYLE, style);
		SetWindowPos(
			*pHwnd,
			HWND_TOPMOST,
			0,
			0,
			width,
			height,
			SWP_SHOWWINDOW | SWP_FRAMECHANGED
		);

		ShowWindow(*pHwnd, SW_SHOWNORMAL);
		m_displayMode = mode;
	}
}

void ViewSystem::MoveView(float x, float y)
{
	m_translation.x += x;
	m_translation.y += y;
}

void ViewSystem::ZoomView(float zoom, Vector2 origin)
{
	m_scale.x += zoom;
	m_scale.y += zoom;

	m_scaleOrigin = origin;
}

Vector2 ViewSystem::scale()
{
	return m_scale;
}

Vector2 ViewSystem::scaleOrigin()
{
	return m_scaleOrigin;
}

Vector2 ViewSystem::translation()
{
	return m_translation;
}

Rect ViewSystem::viewRect()
{
	RECT rect;
	GetClientRect(*(WindowSystem::get().pRenderHwnd()), &rect);

	//Rect view = Rect(m_translation.x, m_translation.y, (rect.right - rect.left + m_translation.x), (rect.bottom - rect.top + m_translation.y));

	/*float appendX = m_scale.x > 1.0f ? view.right * -1 * m_scale.x : view.right * m_scale.x;
	float appendY = m_scale.y > 1.0f ? view.bottom * -1 * m_scale.y : view.bottom * m_scale.y;

	view.right += appendX;
	view.bottom += appendY;*/

	Rect view = Rect(m_translation.x, m_translation.y, (m_worldSize.x + m_translation.x), (m_worldSize.y + m_translation.y));

	return view;
}

bool ViewSystem::focused()
{
	return  m_focused;
}