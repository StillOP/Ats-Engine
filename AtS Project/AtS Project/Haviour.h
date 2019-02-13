#pragma once

class GameObject;

class Haviour
{
	friend class GameObjectsCollection;
	friend class Behaviour;

public:
	~Haviour() {}

private:
	Haviour()
		: m_unbindUpdate(false), m_unbindStateUpdate(false)
	{}

	bool m_unbindUpdate;
	bool m_unbindStateUpdate;
};