#pragma once

class GameObject;

class Component
{
public:
	Component() : m_owner{ nullptr } {}

	virtual ~Component() = 0 { m_owner = nullptr; };


	void owner(GameObject* gameobject) 
	{ 
		if (!m_owner) { m_owner = gameobject; }
	}
	
	GameObject* owner()
	{
		return m_owner;
	}

protected:
	GameObject* m_owner;
};