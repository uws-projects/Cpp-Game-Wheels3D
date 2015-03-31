#pragma once
#include "StateMachine.h"

class Application
{
public:
	static Application* Instance()
	{
		if (m_instance == 0)
		{
			m_instance = new Application();
			return m_instance;
		}

		return m_instance;
	}

	StateMachine* GetStateMachine(){ return m_stateMachine; }

	bool Initialize();
	bool IsRunning() { return m_isRunning; }
	
	void Render();
	void Update();
	void HandleEvents();

	void Quit();

private:

	static Application* m_instance;
	bool m_isRunning;

	SDL_Window* window;
	SDL_GLContext context;

	Application() {}

	StateMachine* m_stateMachine;
};

