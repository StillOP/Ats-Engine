#pragma once
#include <d2d1.h>
#include "DisplayMode.h"
#include "Math.h"

class ViewSystem
{
public:
	~ViewSystem();

	void Init();
	void Update(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


	static ViewSystem& get()
	{
		static ViewSystem vwSys;
		return vwSys;
	}

	void ChangeDisplayMode(DISPLAY_MODE mode);

	void ZoomView(float zoom, Vector2 origin = Vector2(0.0f, 0.0f));
	void MoveView(float x, float y);

	Vector2 scale();
	Vector2 scaleOrigin();
	Vector2 translation();
	Rect viewRect();

	bool focused();

private:
	ViewSystem();

	DISPLAY_MODE m_displayMode;

	RECT m_windowRect;

	Vector2 m_scale;
	Vector2 m_scaleOrigin;
	Vector2 m_translation;

	Vector2 m_worldSize;
	
	bool m_focused;
};