#include "stdafx.h"
#include <string>
#include "InputSystem.h"
#include "ViewSystem.h"


InputSystem::InputSystem() {}

InputSystem::~InputSystem() {}

void InputSystem::Init() {}

void InputSystem::Update(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_KEYDOWN)
	{
		m_keysDownThisFrame[wParam] = true;
		m_keysUpThisFrame[wParam] = false;
	}
	else if (message == WM_KEYUP)
	{
		m_keysUpThisFrame[wParam] = true;
		m_keysDownThisFrame[wParam] = false;
	}
}

void InputSystem::SetKeyAlias(std::string&& alias, unsigned int key)
{
	m_keysAlias[alias] = key;
}

bool InputSystem::IsKeyPressed(unsigned int key)
{
	return GetAsyncKeyState(key) & 0x8000 ? true : false;
}

bool InputSystem::IsKeyJustReleased(unsigned int key)
{
	auto itr = m_keysUpThisFrame.find(key);

	if (itr != m_keysUpThisFrame.end()) { return m_keysUpThisFrame[key]; }

	return false;
}

bool InputSystem::IsKeyPressed(std::string&& alias)
{
	auto itr = m_keysAlias.find(alias);

	if (itr == m_keysAlias.end()) { return false; }

	return GetAsyncKeyState(itr->second) & 0x8000 ? true : false;
}

bool InputSystem::IsKeyJustReleased(std::string&& alias)
{
	auto itr1 = m_keysAlias.find(alias);

	if (itr1 == m_keysAlias.end()) { return false; }

	auto itr2 = m_keysUpThisFrame.find(itr1->second);

	if (itr2 != m_keysUpThisFrame.end()) { return m_keysUpThisFrame[itr2->first]; }

	return false;
}