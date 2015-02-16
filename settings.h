#pragma once

#include <GL\glew.h>
#include <SDL.h>
#include <math.h>
#include <string>

#include "Load.h"
#include "Shader.h"
#include "InputHandler.h"

#include <iostream>
#include "Light.h"
#include "Cube.h"
//#include "Camera.h"
#include "Material.h"

//#include "Wheel.h"

#ifndef RADIANS
#define RADIANS 0.017453293
#endif

#ifndef WIDTH					// width of the screen
#define WIDTH 1280.0f			
#endif

#ifndef HEIGHT					// height of the screen
#define HEIGHT 720.0f
#endif

#ifndef CENTER					// used for centering window position
#define CENTER -1				// improves readability
#endif

#ifndef XCENTRE					// centre of the screen (horizontally)
#define XCENTRE WIDTH/2
#endif

#ifndef YCENTRE					// centre of the screen (vertically)
#define YCENTRE HEIGHT/2
#endif

#ifndef PI
#define PI 3.14159265359f
#endif

// joystick buttons
#define BUTTON_A 10
#define BUTTON_B 11
#define BUTTON_X 12
#define BUTTON_Y 13
#define LB 8
#define RB 9
#define BUTTON_SELECT 5
#define BUTTON_START 4
#define LS 6
#define RS 7
#define JOYSTICK1 0
#define JOYSTICK2 1
#define LEFTSTICK 1
#define RIGHTSTICK 2

//#define FULLSCREEN true
#define FULLSCREEN false
