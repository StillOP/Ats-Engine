#pragma once
#include <string>
#include <d2d1.h>

enum class INPUT_TYPE
{
	KEYUP = WM_KEYUP,
	KEYDOWN = WM_KEYDOWN,
	KEYPRESSED = WM_APP + 1,
	UNDEFINED = -1
};

struct Input
{
	Input()
	{
		keyCode = -1;
	}

	unsigned int keyCode;
};