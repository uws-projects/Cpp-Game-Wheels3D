#include "Application.h"
#include "Play.h"

Application* Application::m_instance = 0;

bool Application::Initialize()
{
	//SDL_Window * window;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize video
	{
		std::cerr << "Unable to initialize SDL\n";
		return false;
	}

	// Request an OpenGL 3.0 context.

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // double buffering on
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // 8 bit alpha buffering
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // Turn on x4 multisampling anti-aliasing (MSAA)

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	int flags = false;
	if (FULLSCREEN)
		flags = SDL_WINDOW_FULLSCREEN;

	// Create 800x600 window
	window = SDL_CreateWindow("Wheels 3D", 400, SDL_WINDOWPOS_CENTERED,
		1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | flags) ;
	if (!window) // Check window was created OK
	{
		std::cerr << "Unable to create window\n";
		return false;
	}

	context = SDL_GL_CreateContext(window); // Create opengl context and attach to window
	SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate

	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) { // glewInit failed, something is seriously wrong
			{
				std::cerr << "glewInit failed, aborting\n";
				exit(1);
				return false;		// should not reach here anyway
			}
	}
	std::cout << glGetString(GL_VERSION) << "\n";

	m_isRunning = true;
	m_stateMachine = new StateMachine();
	m_stateMachine->PushState(new Play());
	return true;
}

void Application::Update()
{
	INPUT->Update();
	SDL_ShowCursor(false);
	m_stateMachine->Update();
	
	SDL_WarpMouseInWindow(window, (int) XCENTRE, (int) YCENTRE);
}

void Application::Render()
{
	m_stateMachine->Render();
	SDL_GL_SwapWindow(window);
}

void Application::HandleEvents()
{
	m_stateMachine->HandleEvents();
	
}

void Application::Quit()
{
	m_isRunning = false;
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}