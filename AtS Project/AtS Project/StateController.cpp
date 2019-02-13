#include "stdafx.h"
#include "StateController.h"



StateController::StateController()
	: m_stackMaxSize(4)
{}

StateController::~StateController() 
{}

void StateController::AddState(unsigned int state)
{
	for (auto& itr : m_stateStack)
	{
		if (itr == state) { return; }
	}

	m_stateStack.push_front(state);
}

void StateController::AddStateRange(std::vector<unsigned int>&& states)
{
	for (auto& stateIt : states)
	{
		for (auto& stateStackIt : m_stateStack)
		{
			if (stateStackIt == stateIt) { continue; }
		}

		m_stateStack.push_front(stateIt);
	}
}

void StateController::SetState(unsigned int state)
{
	if (m_stateStack.empty()) { return; }

	unsigned int currentState = m_stateStack[0];

	for (auto stateIt =  m_stateStack.begin(); stateIt != m_stateStack.end();)
	{
		if (*stateIt == state)
		{
			(*m_behaviour)->OnStateExit(currentState);

			stateIt = m_stateStack.erase(stateIt);

			(*m_behaviour)->OnStateEntered(state);

			break;
		}
		else { ++stateIt; }
	}
	m_stateStack.push_front(state);
}

void StateController::BackToPreviousState()
{
	if (m_stateStack.size() < 2) { return; }

	unsigned int previousState = m_stateStack[m_stateStack.size() - 2];

	SetState(previousState);
}

void StateController::SetBehaviour(Behaviour** behaviour) { m_behaviour = behaviour; }

int StateController::currentState()
{
	if (m_stateStack.empty()) { return -1; }

	return m_stateStack[0];
}

void StateController::stackSize(int size)
{
	if (size < 2) { return; }

	m_stackMaxSize = size;
}