// This application was created by NT Games for the Design and plan module
// using rt3d utility library at core provided by Daniel Livingstone
// Uses OpenGL, SDL, GLM and BASS
// november 2014
// Author: NT Games (Constantin Toader & Daniel Richardson)
// change resolution in settings.h file



// Windows specific: Uncomment the following line to open a console window for debug output
//#if _DEBUG
//#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
//#endif

#pragma comment(linker, "/entry:\"WinMainCRTStartup\"")

const int FPS = 60;
const float DELAYTIME = 1000.0f / FPS;
#include "Application.h"

// toggle fullscreen on/off in the settings header file

using namespace std;

int main(int argc, char *argv[]) {

	Uint32 frameStart, frameTime;
	if (Application::Instance()->Initialize())
	{
		while (Application::Instance()->IsRunning())
		{
			frameStart = SDL_GetTicks();
			Application::Instance()->HandleEvents();
			Application::Instance()->Update();
			Application::Instance()->Render();
		
			frameTime = SDL_GetTicks() - frameStart;
			if (frameTime < DELAYTIME)						// fps sync
			{
				SDL_Delay((int)(DELAYTIME - frameTime));
			}
		}
	}
	else
	{
		std::cerr << "The application failed to intialise..." << SDL_GetError() << "\n";
		return -1;
	}
	return 0;
}
