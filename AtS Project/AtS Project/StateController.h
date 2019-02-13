#pragma once
#include <string>
#include <functional>
#include <deque>
#include <map>
#include <vector>
#include "Behaviour.h"


class StateController : public Component
{
public:
	StateController();
	~StateController();

	void AddState(unsigned int state);
	void AddStateRange(std::vector<unsigned int>&& states);

	void SetState(unsigned int state);
	void BackToPreviousState();

	void SetBehaviour(Behaviour** behaviour);
	
	int currentState();
	void stackSize(int size);

private:
	std::deque<unsigned int> m_stateStack;
	int m_stackMaxSize;

	Behaviour** m_behaviour;
};