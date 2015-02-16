#pragma once
#include "AbstractState.h"

class StateMachine
{
private:

	std::vector<AbstractState*> m_gameStates;

public:

	StateMachine(void) {}
	~StateMachine(void) { Clean(); }

	void PushState(AbstractState* state);		// will add a state without removing the previous state
	void PopState();							// will remove the current state without adding another

	void Update();
	void Render();
	void HandleEvents();

	void Clean();
};