#include "StateMachine.h"

void StateMachine::PushState(AbstractState* state)
{
	m_gameStates.push_back(state);
	m_gameStates.back()->OnEnter();
}

void StateMachine::PopState()
{
	if (!m_gameStates.empty())
	{
		if (m_gameStates.back()->OnExit())
		{
			delete m_gameStates.back();
			m_gameStates.pop_back();
		}
	}
}

void StateMachine::Update()
{
	if (!m_gameStates.empty())
	{
		m_gameStates.back()->Update();
	}
}

void StateMachine::Render()
{
	if (!m_gameStates.empty())
	{
		m_gameStates.back()->Render();
	}
}

void StateMachine::HandleEvents()
{
	if (!m_gameStates.empty())
	{
		m_gameStates.back()->HandleEvents();
	}
}

void StateMachine::Clean()
{
	if (!m_gameStates.empty())
		for (unsigned int i = 0; i != m_gameStates.size(); i++)
		{
		delete m_gameStates[i];
		}
}