#pragma once
#include "InputHandler.h"
#include "Application.h"



InputHandler* InputHandler::s_pInstance = 0;
float InputHandler::m_MouseSensitivity = 1.0f;
bool InputHandler::axisMoved = false;
// ------------------------------- Edit Sensibility here --------------------------------
float InputHandler::m_joystickSensibility = 0.1f;		// edit sensibility here
// ------------------------------ Edit Neutral Area here --------------------------------
const int m_joystickDeadZone = 8000;
#ifndef MOVEMENTVALUE
#define MOVEMENTVALUE 1
#endif
#ifndef RESETPOSITION
#define RESETPOSITION 0
#endif



InputHandler::InputHandler()
{
	// init mouse by default to not pressed
	for(int i = 0; i < 3; i++) 
	{  
		m_mouseButtonStates.push_back(false); 
	} 
	m_mousePosition = new glm::vec2();
}

// ------------------------------------ Functionality -----------------------------------
void InputHandler::InitialiseJoysticks()
{
	
	if (SDL_WasInit(SDL_INIT_JOYSTICK) == 0)
		// joystick init check
	{
		SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	}

	if (SDL_NumJoysticks() > 0)
	{
		for (int i=0; i < SDL_NumJoysticks(); i++)
		{
			SDL_Joystick* joy = SDL_JoystickOpen(i);
			// open all the available joysticks

			if (joy)
				// if opened
			{
				// push in the array
				m_joysticks.push_back(joy);
				m_joystickValues.push_back(std::make_pair
					(new glm::vec2(), new glm::vec2()));
				m_triggerValues.push_back(0);

				// we use SDL_JoystickNumButtons to get the number of buttons
				// for each of our joysticks and then push a value for each of 
				// these buttons into an array, by default no buttons are pressed
				// so false;
				std::vector<bool> tempButtons;

				for(int j=0; j < SDL_JoystickNumButtons(joy); j++)
				{
					tempButtons.push_back(false);
				}

				m_buttonStates.push_back(tempButtons);
			}
			else
			{
				// else find out why
				std::cout << SDL_GetError << "\n";
			}
		}
		SDL_JoystickEventState(SDL_ENABLE);
		// start listening for joystick events
		m_bJoysticksInitialised = true;

		std::cout << "Initialised " << m_joysticks.size() << " joystick(s)\n";
	}
	else
	{
		m_bJoysticksInitialised = false;
	}
}

void InputHandler::Clean()
// method to go through the array and close the opened joysticks
{
	if (m_bJoysticksInitialised)
	{
		for (int i = 0; i < SDL_NumJoysticks(); i++)
		{
			SDL_JoystickClose(m_joysticks[i]);
		}
	}
}

void InputHandler::Update()
{
	m_keystate = SDL_GetKeyboardState(NULL);
	SDL_Event event;
	while (SDL_PollEvent(&event)) 
	{
		switch (event.type)
		{
		case SDL_QUIT:				Application::Instance()->Quit();break;
		case SDL_JOYAXISMOTION:		onJoystickAxisMove(event);		break;
		case SDL_JOYBUTTONDOWN:		onJoystickButtonDown(event);	break;
		case SDL_JOYBUTTONUP:		onJoystickButtonUp(event);		break;
		case SDL_MOUSEMOTION:		onMouseMove(event);				break;
		case SDL_MOUSEBUTTONDOWN:	onMouseButtonDown(event);		break;
		case SDL_MOUSEBUTTONUP:		onMouseButtonUp(event);			break;
	//	case SDL_KEYDOWN:			onKeyDown();					break;
	//	case SDL_KEYUP:				onKeyUp();						break;
		default: break;
		}
	}
}

void InputHandler::onJoystickAxisMove(SDL_Event& event)
{
// ------------------------------- event is axis movement -------------------------------
		if (event.type == SDL_JOYAXISMOTION)
			// check the type value
		{
			axisMoved = true;
			int whichOne = event.jaxis.which;
			//std::cout << event.jaxis.value << "\n";
			// and get which controler

#define LEFT_STICK_X 0
#define LEFT_STICK_Y 1
#define RIGHT_STICK_X 2
#define RIGHT_STICK_Y 3
#define LEFT_TRIGGER 4
#define RIGHT_TRIGGER 5
			// axis: 0 = X AXIS (left stick, left/right movement)
			// axis: 1 = Y AXIS (left stick, up/right movement)
			// axis: 2 = X ROTATION AXIS (right stick, left/right movement)
			// axis: 3 = Y ROTATION AXIS (right stick, up/down movement)
			// axis: 4 = Z- ROTATION AXIS (left trigger)
			// axis: 5 = Z+ ROTATION AXIS (right trigger)
			// change here the amount of movement by setting different parameter

// ----------------------- Checking movement for each axis ------------------------------
			if (event.jaxis.axis == LEFT_STICK_X)
			{
				// searching for X AXIS MOVEMENT
				// left stick horizontal movement
				
				if (event.jaxis.value > m_joystickDeadZone || event.jaxis.value < -m_joystickDeadZone)
				{
					leftStickX = event.jaxis.value;
					m_joystickValues[whichOne].first->x = leftStickX;
					//std::cout << "Axis 0: LEFT STICK X AXIS: " << event.jaxis.value << "\n";

				}
				else
					// else don`t move
				{
					m_joystickValues[whichOne].first->x = RESETPOSITION;
				}
			}
					

			if (event.jaxis.axis == LEFT_STICK_Y) 
			{
				// searching for Y AXIS MOVEMENT
				// left stick vertical movement
				
				if (event.jaxis.value > m_joystickDeadZone || event.jaxis.value < -m_joystickDeadZone)
				{
					leftStickY = event.jaxis.value;
					m_joystickValues[whichOne].first->y = leftStickY;
					//std::cout << "Axis 1: LEFT STICK Y AXIS: " << event.jaxis.value << "\n";
				}
				else
					// else don`t move
				{
					m_joystickValues[whichOne].first->y = RESETPOSITION;
				}
			}


			if (event.jaxis.axis == RIGHT_STICK_X)
			{
				// searching for X ROTATION MOVEMENT
				// right stick horizontal movement
								
				if (event.jaxis.value > m_joystickDeadZone || event.jaxis.value < -m_joystickDeadZone)
				{
					rightStickX = event.jaxis.value;
					m_joystickValues[whichOne].second->x = rightStickX;
					//std::cout << "Axis 2: RIGHT STICK X AXIS: " << event.jaxis.value << "\n";
				}
				else
					// else don`t move
				{
					m_joystickValues[whichOne].second->x = RESETPOSITION;
				}
			}

			if (event.jaxis.axis == RIGHT_STICK_Y)
			{
			 // searching for Y ROTATION MOVEMENT
			 // right stick horizontal movement
		
				if (event.jaxis.value > m_joystickDeadZone || event.jaxis.value < -m_joystickDeadZone)
				{
					rightStickY = event.jaxis.value;
					m_joystickValues[whichOne].second->y = rightStickY;
					//std::cout << "Axis 3: Y ROTATION: " << event.jaxis.value << "\n";
				}
				else
					// else don`t move
				{
					m_joystickValues[whichOne].second->y = RESETPOSITION;
				}
			}

			if (event.jaxis.axis == LEFT_TRIGGER)
			{
				// searching for TRIGGER MOVEMENT
				// LT 

				m_triggerValues[whichOne] = (event.jaxis.value + 32768) / 2.0f;

				if (m_triggerValues[whichOne] < 1)
				{
					m_triggerValues[whichOne] = RESETPOSITION;
				}
			}

			if (event.jaxis.axis == RIGHT_TRIGGER)
			{
				// searching for TRIGGER MOVEMENT
				// RT

				m_triggerValues[whichOne] = (event.jaxis.value + 32768) / 2.0f;

				if (m_triggerValues[whichOne] < 1)
				{
					m_triggerValues[whichOne] = RESETPOSITION;
				}
				else
					m_triggerValues[whichOne] = -m_triggerValues[whichOne];

			}
			
		}
		else
		{
			axisMoved = false;
		}
// -------------------------------- end of axis movement --------------------------------
}

void InputHandler::onJoystickButtonDown(SDL_Event& event)
{
// ----------------------------- event is joy button press ------------------------------
		if (event.type == SDL_JOYBUTTONDOWN)
			// so when a button is pressed find on which joystick which
			// is the button id and set according array value to true
		{
			int whichOne = event.jaxis.which;
			m_buttonStates[whichOne][event.jbutton.button] = true;
		//	std::cout << "button : " << event.button.which << "\n";
		}
// ------------------------------ end of joy button press -------------------------------
}

void InputHandler::onJoystickButtonUp(SDL_Event& event)
{
// ----------------------------- end of joy button release ------------------------------
		if (event.type == SDL_JOYBUTTONUP)
			// when a button is released find on which joystick which
			// is the button id and set according array value to false
		{
			int whichOne = event.jaxis.which;

			m_buttonStates[whichOne][event.jbutton.button] = false;
		}
// ----------------------------- end of joy button release ------------------------------
}

void InputHandler::onMouseButtonDown(SDL_Event& event)
{
// ----------------------------- event is mouse button press ----------------------------
// click
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				m_mouseButtonStates[LEFT] = true;
			}

			if(event.button.button == SDL_BUTTON_MIDDLE)
			{
				m_mouseButtonStates[MIDDLE] = true;
			}

			if(event.button.button == SDL_BUTTON_RIGHT)
			{
				m_mouseButtonStates[RIGHT] = true;
			}
		}
// ---------------------------- end of mouse button release ------------------------------

}

void InputHandler::onMouseButtonUp(SDL_Event& event)
{
// ------------------------------- event is mouse release -------------------------------
// declick
		if (event.type == SDL_MOUSEBUTTONUP)
		{
			if(event.button.button == SDL_BUTTON_LEFT)
			{
				m_mouseButtonStates[LEFT] = false;
			}

			if(event.button.button == SDL_BUTTON_MIDDLE)
			{
				m_mouseButtonStates[MIDDLE] = false;
			}

			if(event.button.button == SDL_BUTTON_RIGHT)
			{
				m_mouseButtonStates[RIGHT] = false;
			}
		}
// ---------------------------- end of mouse button release -----------------------------
}

void InputHandler::onMouseMove(SDL_Event& event)
{
// ------------------------------ event is mouse movement -------------------------------
		if (event.type == SDL_MOUSEMOTION)
		{
			m_mousePosition->x = (float)event.motion.x;
			m_mousePosition->y = (float)event.motion.y;
		}
// ------------------------------- end of mouse movement --------------------------------
}

bool InputHandler::OnKeyDown(SDL_Scancode key)
{
	return IsKeyPressed(key);
}

bool InputHandler::OnKeyUp(SDL_Scancode key)
{
	return !IsKeyPressed(key);
}

float InputHandler::xValue(int joy, int stick)
	// method to get X value from the respective device and stick
{
	if (m_joystickValues.size() > 0)
	{
		if (stick == 1)
			// stick 1 is left stick; stick 2 is right stick
		{
			return (m_joystickValues[joy].first->x - m_joystickDeadZone); // 364.0f;
		}
		else if (stick == 2)
		{
			return (m_joystickValues[joy].second->x - m_joystickDeadZone); // 364.0f;
		}
	}
	return 0;
}

float InputHandler::yValue(int joy, int stick)
	// method to get Y value from the respective device and stick
{
	if (m_joystickValues.size() > 0)
	{
		if (stick == 1)
			// stick 1 is left; stick 2 is right
		{
			return (m_joystickValues[joy].first->y - m_joystickDeadZone); // 364.0f;
		}
		else if (stick == 2)
		{
			return (m_joystickValues[joy].second->y - m_joystickDeadZone); // 364.0f;
		}
	}
	return 0;
}

float InputHandler::zValue(int joy)
{
	float value = m_triggerValues[joy];
	if (value < 0)
		rightTrigger = value / -326;
	else
		leftTrigger = value / 326;
	return m_triggerValues[joy]; // (364.0f*90.0);
}

int InputHandler::clampToPercentage(float value)
{
	float range = (32768 - m_joystickDeadZone);
	if (value == 0) return 0;
	else {
		if (value < 0)
			return (int) (100 * (value + m_joystickDeadZone) / range);
		else
			return (int) (100 * (value - m_joystickDeadZone) / range);
	}
}