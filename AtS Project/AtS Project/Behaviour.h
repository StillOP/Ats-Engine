#pragma once
#include "Haviour.h"
#include "Component.h"

class Behaviour: public Component
{
public:
	Behaviour(Haviour haviour)
		: m_haviour{ haviour } {}

	virtual ~Behaviour() {}

	virtual void Init() = 0;

	virtual void Update() 
	{
		m_haviour.m_unbindUpdate = true;
	}

	virtual void OnStateEntered(unsigned int state) {}

	virtual void OnStateUpdate(unsigned int state)
	{
		m_haviour.m_unbindStateUpdate = true;
	}

	virtual void OnStateExit(unsigned int state) {}

	const Haviour* haviour() { return &m_haviour; }
private:
	Haviour m_haviour;
};