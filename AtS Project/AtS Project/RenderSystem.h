#pragma once
#include <d2d1.h>

class RenderSystem
{
public:
	~RenderSystem();

	static RenderSystem& get()
	{
		static RenderSystem renderSys;
		return renderSys;
	}

	void Init();
	void Render(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	RenderSystem();
};