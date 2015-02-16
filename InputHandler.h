// This is a reusable Input handler created by Constantin Toader
// after the examples from SDL Game Development book by Shaun Mitchel
// It includes handling input from devices like keyboard, mouse and joystick


// updated on 08/12/2014 
// added possibility to return axis values as percentage or degrees.

#ifndef __InputHandler__
#define __InputHandler__

#ifndef PRESSING
#define PRESSING TheInputHandler::Instance()->IsKeyPressed
#endif

#ifndef INPUT
#define INPUT TheInputHandler::Instance()
#endif

#ifndef ISRELEASED
#define ISRELEASED TheInputHandler::Instance()->OnKeyUp
#endif

#ifndef JOY
#define JOY InputHandler::Instance()
#endif

// shortcut calls for axis in percentage values
// from 0 to 100

#define LEFTXAXIS100 InputHandler::Instance()->LeftX()
#define RIGHTXAXIS100 InputHandler::Instance()->RightX()
#define LEFTYAXIS100 InputHandler::Instance()->LeftY()
#define RIGHTYAXIS100 InputHandler::Instance()->RightY()
#define LEFTTRIGGER100 InputHandler::Instance()->LeftTrigger()
#define RIGHTTRIGER100 InputHandler::Instance()->RightTrigger()

// shortcut calls for current joystick in degrees values
// from 0 to 90

#define LEFTXAXIS90 LEFTXAXIS100*0.9f
#define RIGHTXAXIS90 RIGHTXAXIS100*0.9f
#define LEFTYAXIS90 LEFTYAXIS100*0.9f
#define RIGHTYAXIS90 RIGHTXAXIS100*0.9f
#define LEFTTRIGGER90 LEFTTRIGGER100*0.9f
#define RIGHTTRIGGER90 RIGHTTRIGER100*0.9f

#define JOY_SELECT (JOY->GetButtonState(JOYSTICK1, BUTTON_SELECT))
#define JOY_START (JOY->GetButtonState(JOYSTICK1, BUTTON_START))
#define JOY_A (JOY->GetButtonState(JOYSTICK1, BUTTON_A))
#define JOY_B (JOY->GetButtonState(JOYSTICK1, BUTTON_B))
#define JOY_X (JOY->GetButtonState(JOYSTICK1, BUTTON_X))
#define JOY_Y (JOY->GetButtonState(JOYSTICK1, BUTTON_Y))
#define JOY_LB (JOY->GetButtonState(JOYSTICK1, LB))
#define JOY_RB (JOY->GetButtonState(JOYSTICK1, RB))
#define JOY_LS (JOY->GetButtonState(JOYSTICK1, LS))
#define JOY_RS (JOY->GetButtonState(JOYSTICK1, RS))


#include <SDL.h>
#include <vector>
#include <iostream>
#include "glm.hpp"
/*
*		This class is a singleton that handles all deveice 
*		input, whether it is from controllers, keyboard or mouse
*/

enum mouseButtons
	// enum used to easily identify which button of a mouse
	// performs an action
{
	LEFT = 0,
	MIDDLE = 1,
	RIGHT = 2,
};

class InputHandler
{
public:

	static InputHandler* Instance()
	{
		if(s_pInstance == 0)
		{
			s_pInstance = new InputHandler();
			return s_pInstance;
		}

		return s_pInstance;
	}
	static float m_joystickSensibility;

	void InitialiseJoysticks();
	void Update();
	void Clean();

	static bool axisMoved;
	float xValue(int joy, int stick);
	float yValue(int joy, int stick);
	float zValue(int joy);

	inline bool JoysticksInitialised()						{ return m_bJoysticksInitialised; }
	inline bool GetButtonState(int joy, int buttonNumber)	{ return m_buttonStates[joy][buttonNumber]; }
	inline bool GetMouseButtonState(int buttonNumber)		{ return m_mouseButtonStates[buttonNumber]; }
	inline glm::vec2* GetMousePosition()					{ return m_mousePosition; }
	inline bool IsKeyPressed(SDL_Scancode key) 
	{
		if (SDL_GetKeyboardState(NULL) != 0)
		{
			if (SDL_GetKeyboardState(NULL)[key] == 1) return true;	else return false;
		}
		return false;
	}
	
	// handle keyboard events
	bool OnKeyUp(SDL_Scancode key);
	bool OnKeyDown(SDL_Scancode key);
	std::vector<std::pair<glm::vec2*, glm::vec2*>> m_joystickValues;
	std::vector<float> m_triggerValues;

	float& MouseSensitivity() { return m_MouseSensitivity; }

	// axis accessors
	int LeftX() { return (int) clampToPercentage(leftStickX); }
	int LeftY() { return (int) clampToPercentage(leftStickY); }
	int RightX() { return (int) clampToPercentage(rightStickX); }
	int RightY() { return (int) clampToPercentage(rightStickY); }
	int LeftTrigger() { return (int)leftTrigger; }
	int RightTrigger() { return (int)rightTrigger; }

private:
	static InputHandler* s_pInstance;
	static float m_MouseSensitivity;
	
	InputHandler(void);
	~InputHandler(void) {}

	bool m_bJoysticksInitialised;
	std::vector<SDL_Joystick*> m_joysticks; 
	// will know how many joysticks SDL has access to
	
	// use vec2 values to set whether a stick has moved up, down, left or right
	std::vector<std::vector<bool>> m_buttonStates;
	// array of boolean values to be used by each controller 
	// first is the joystick id and second is an array of booleans 
	// values for each button on each controller
	std::vector<bool> m_mouseButtonStates;
	// same principle but only one array is needed as there is only 
	// one mouse so no index needed, but the array to hold the booleans
	// for each mouse button
	glm::vec2* m_mousePosition;
	const Uint8* m_keystate;					
	
	// handle mouse vents
	void onMouseMove(SDL_Event& event);
	void onMouseButtonDown(SDL_Event& event);
	void onMouseButtonUp(SDL_Event& event);

	// handle joystick events
	void onJoystickAxisMove(SDL_Event& event);
	void onJoystickButtonDown(SDL_Event& event);
	void onJoystickButtonUp(SDL_Event& event);

	int clampToPercentage(float value);
	float clampToDegree(int value);

	// joystick axis values
	float leftStickX;
	float leftStickY;
	float rightStickX;
	float rightStickY;
	float trigger;
	float leftTrigger;
	float rightTrigger;

};

typedef InputHandler TheInputHandler;

#endif // defined (__InputHandler__)