#pragma once
#include <map>
#include <functional>
#include "Input.h"



class InputSystem
{
public:
	~InputSystem();

	void Init();
	void Update(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	static InputSystem& get()
	{
		static InputSystem inputSys;
		return inputSys;
	}

	void SetKeyAlias(std::string&& alias, unsigned int key);

	bool IsKeyPressed(unsigned int key);
	bool IsKeyJustReleased(unsigned int key);

	bool IsKeyPressed(std::string&& alias);
	bool IsKeyJustReleased(std::string&& alias);

private:
	InputSystem();

	std::map<unsigned int, bool> m_keysDownThisFrame;
	std::map<unsigned int, bool> m_keysUpThisFrame;

	std::map<std::string, unsigned int> m_keysAlias;
};