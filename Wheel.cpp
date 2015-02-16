#pragma once
#include "Wheel.h"

Wheel::Wheel()
{
}

void Wheel::Initialize()
{
	firstPersonCamera.Initialize();
	model = Load::Obj(".\\model\\wheels_ultimul.obj");
	indexCount = Load::meshCount();
	material.Ka = glm::vec4(0.0, 0.0, 0.0, 1.0);
	material.Kd = glm::vec4(0.7, 0.7, 0.7, 1.0);
	material.Ks = glm::vec4(0.33, 0.33, 0.33, 1.0);
	material.shininess = 0.66f;

	direction = 90;
	rotationAngle = 50;
	Acceleration = 0;
	rubber = Load::BMP(".\\textures\\rubber.bmp");
	scale = glm::vec3(1.0f);
	position = glm::vec3(5.0f, RADIUS - tyreAspectRatio, 5.0f);
	lastPosition = position;

	cameraHeight = position.y*2.5f;
	cameraZoom = 2.0f;
	editingCameraPosition = false;
	hugeZoom = false;
	zoomAmount = 0.1f;

	initializePhysics();

	leftCollisionMap = Load::CollisionMap(".\\contact\\leftMap.obj");
	rightCollisionMap= Load::CollisionMap(".\\contact\\rightMap.obj");

	glEnable(GL_CULL_FACE);
}



void Wheel::Render()
{

	Shader::Push();
	{
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		Shader::Top() = glm::translate(Shader::Top(), position);
		Shader::Top() = glm::rotate(Shader::Top(), -direction, glm::vec3(0.0f, 1.0f, 0.0f));
		Shader::Top() = glm::rotate(Shader::Top(), rotationAngle, glm::vec3(1.0f, 0.0f, 0.0f));
		Shader::Top() = glm::scale(Shader::Top(), glm::vec3(0.5f));
		Shader::Use(Shader::DiffusePerVertex());
		Shader::SetUniform("ModelViewMatrix", Shader::Top());
		Shader::SetUniform("NormalMatrix", Shader::NormalMatrix());
		Shader::SetUniform("ProjectionMatrix", Shader::ProjectionMatrix());
		Shader::SetUniform("MVP", Shader::ProjectionMatrix()*Shader::Top());
		// TO DO
		// when texture coordinates will be created bind this
		//Shader::Bind(0, "tex", rubber);
		glBindVertexArray(model);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	
	}
	Shader::Pop();
}

bool Wheel::contacts(float& left, float& right, float& top, float &bottom, float& x, float &z) {

	// if is outside the box... then no collision
	if (x<left - RADIUS)		return false;
	if (x>right + RADIUS)		return false;
	if (z>bottom + RADIUS)		return false;
	if (z<top - RADIUS)			return false;

	std::cout << "contact at X: "<< x << " Z: "<< z << "\n";
	std::cout << "\n left:" << left << "\tright :" << right << "\ttop :" << top << "\tbottom :" << bottom <<"\n";
	glm::vec3 response = glm::vec3(x, RADIUS, z) - lastPosition;

	if (lastPosition.x < left || lastPosition.x > right)
	{
		response.x *= -1;
	}
	if (lastPosition.z < bottom || lastPosition.z > top)
	{
		response.z *= -1; 
	}


	response = glm::normalize(response);

	position.x += response.x;
	position.z += response.z;
	
	return true;
}

void Wheel::checkCollision()
{
	if (position.z < 130.0f)
		// left map is from 0 to 130 on Z axis
	{
		for (unsigned int i = 0; i < leftCollisionMap.size() - 1; i += 4)
			// so read the coordinates of the two points, so two pairs of x,y 
			// that`s why we increase by 4 at each step
		{
			// check for collision between the rectangle described by the 2 points
			// they are already sorted from the loading function
			// and check against the current position of the player
			contacts(leftCollisionMap[i],
				leftCollisionMap[i + 1],
				leftCollisionMap[i + 2],
				leftCollisionMap[i + 3],
				position.x,
				position.z);
		}
	}
	else
	{
		for (unsigned int i = 0; i < rightCollisionMap.size() - 1; i += 4)
			// so read the coordinates of the two points, so two pairs of x,y 
			// that`s why we increase by 4 at each step
		{
			// check for collision between the rectangle described by the 2 points
			// they are already sorted from the loading function
			// and check against the current position of the player
			contacts(rightCollisionMap[i],
				rightCollisionMap[i + 1],
				rightCollisionMap[i + 2],
				rightCollisionMap[i + 3],
				position.x,
				position.z);
		}
	}
}

glm::vec3 moveForward(glm::vec3 pos, GLfloat angle, GLfloat d, GLfloat rotation) {
	return glm::vec3(pos.x + d*std::sin(angle*DEGREES), pos.y + d*std::sin(rotation*DEGREES), pos.z - d*std::cos(angle*DEGREES));
}

void Wheel::gearUp()
{
	if (currentGear < 6) currentGear++;
}

void Wheel::gearDown()
{
	if (currentGear > 1) currentGear--;
}

void Wheel::Update()
{
	rotationAngle += Velocity / 2 * PI;
	if (rotationAngle > 360) rotationAngle -= 360;
	
	if (position.z < RADIUS)	{ position.z = RADIUS; }
	if (position.z > 250.0f - RADIUS) { position.z = 250.0f - RADIUS; }
	if (position.x < RADIUS)	{ position.x = RADIUS; }
	if (position.x > 190.0f - RADIUS) { position.x = 190.0f - RADIUS; }

	checkCollision();
}

void Wheel::initializePhysics()
{
	currentGear = 1;
	Velocity = 0.0f;
	Acceleration = 0.0f;
	F_traction = 0.0f;
	F_long = 0.0f;
	F_drag = 0.0f;
	F_rr = 0.0f;
	F_braking = 0.0f;
	u = 1.0f;
	EnginePower = 0.0f;
	Trigger = 0.0f; 

	canGearDown = true;
	canGearUp = true;
}

void Wheel::updatePhysics()
{
	// clamp direction between 0 and 360 degrees
	if (direction > 360) direction -= 360;
	if (direction < -360) direction += 360;

	// calculate resistance forces
	F_drag = -Cdrag * 0.5f * AREA * RHOAIR * Velocity * Velocity;
	F_rr = -Crolling * Velocity;
	float F_resistance = F_drag + F_rr;

	// calculate traction force
	F_traction = u * EnginePower * currentGear;
	
	// calculate longitudinal force when braking or not
	if (EnginePower < 0) 
	{
		F_braking = -u * Cbraking * abs(EnginePower);
		F_long = F_braking + F_resistance;
	}
	else 
	{
		F_long = F_traction + F_resistance;
	}

	// if the longitudinal force is too close to 0, make it 0	
	if (abs(F_long) < 0.5f) F_long = 0.0f;
	
	Acceleration = F_long / MASS; 
	Velocity = Velocity +  Acceleration * DT ;
	
	// if the velocity is too close to 0, make it 0
	if (abs(Velocity) < 0.04) Velocity = 0.0f;

	// report section, uncomment as necessary
	std::cout << "\nF_long: " << F_long;
	std::cout << "\nAcceleration : " << Acceleration;
	//std::cout << "\nF_traction: " << F_traction;
	//std::cout << "\nEngine Power: " << EnginePower;
	std::cout << "\nVelocity : " << Velocity;
	std::cout << "\nCurrent Gear: " << currentGear;
	
}

void Wheel::HandleEvents()
{
	lastPosition = position;
	if (JOY->JoysticksInitialised())
	{

		float RightStickXvalue = (JOY->xValue(JOYSTICK1, RIGHTSTICK)) / 364 + 19.23123f;
		float RightStickYvalue = (JOY->yValue(JOYSTICK1, RIGHTSTICK)) / 364 + 19.23123f;
		float LeftStickXValue = (JOY->xValue(JOYSTICK1, LEFTSTICK)) / 364 + 19.23123f;

		// record trigger values and invert axis 
		/* by default the right trigger returns negative values 
		*  and left trigger returns positive values, but we want to 
		*  use the commands inverted (RT gas LT brake) so we multiply by -1
		*/
		Trigger = JOY->zValue(JOYSTICK1)*-1;

		// transform joystick axis in percentage values from -100 to 100
		EnginePower = (Trigger / 32765) * 100;

		// value to reduce turning at high speeds
		float toAdd = (LeftStickXValue / 50.0f);// *(1.1f + Velocity*0.8f);

		if (LeftStickXValue > 15.0f || LeftStickXValue < -15.0f) direction += toAdd;
		std::cout << "\n" << LeftStickXValue;
		
		// gear up on release of RB
		if (canGearUp)
		{
			if (JOY_RB) {
				canGearUp = false;
				gearUp();
			}
		}

		// gear down on release of LB
		if (canGearDown)
		{
			if (JOY_LB) {
				canGearDown = false;
				gearDown();
			}
		}

		// update toggles
		if (JOY_RB == SDL_RELEASED) canGearUp = true;
		if (JOY_LB == SDL_RELEASED) canGearDown = true;
		
		// camera controls
		// pressing select enables editing camera position
		if (JOY_SELECT)
		{
			editingCameraPosition = !editingCameraPosition;
			SDL_Delay(150);
		}

		// pressing start enables fast zoom
		if (JOY_START)
		{
			hugeZoom = !hugeZoom;
			SDL_Delay(150);
			if (hugeZoom) zoomAmount = 1.0f;
			else zoomAmount = 0.1f;
		}

		if (editingCameraPosition) {
			if (RightStickYvalue > 10) if (cameraHeight > 0.1f) cameraHeight -= zoomAmount;
			if (RightStickYvalue < -10) cameraHeight += zoomAmount;
			if (RightStickXvalue > 10) if (cameraZoom < 10.0f) cameraZoom += zoomAmount;
			if (RightStickXvalue< -10) if (cameraZoom > 0.1f) cameraZoom -= zoomAmount;
		}
		cameraTurn = -RightStickYvalue;			
	}
	else
	{
		// movement
		if (PRESSING(SDL_SCANCODE_W)) if (EnginePower < 100)  EnginePower+=2.0f; 
		if (PRESSING(SDL_SCANCODE_S)) if (EnginePower > -100) EnginePower-=2.0f;
		if (PRESSING(SDL_SCANCODE_D)) direction++; 
		if (PRESSING(SDL_SCANCODE_A)) direction--;

		// gear shifting 
		// manual
		if (PRESSING(SDL_SCANCODE_1)) currentGear = 1;
		if (PRESSING(SDL_SCANCODE_2)) currentGear = 2;
		if (PRESSING(SDL_SCANCODE_3)) currentGear = 3;
		if (PRESSING(SDL_SCANCODE_4)) currentGear = 4;
		if (PRESSING(SDL_SCANCODE_5)) currentGear = 5;
		if (PRESSING(SDL_SCANCODE_6)) currentGear = 6;
		// automatic
		if (PRESSING(SDL_SCANCODE_Q)) gearDown();
		if (PRESSING(SDL_SCANCODE_E)) gearUp();

		updatePhysics();
		position.x += Velocity * -std::sin(direction*DEGREES) * DT;
		position.z += Velocity * std::cos(direction*DEGREES) * DT;
	}

	updatePhysics();
	// update wheel position
	position.x += Velocity * -std::sin(direction*DEGREES) * DT;
	position.z += Velocity * std::cos(direction*DEGREES) * DT;

	cameraPosition = moveForward(position, direction, 1.0f, cameraTurn);
	cameraPosition.y = cameraHeight;
	cameraAt = glm::vec3(
		position.x + cameraZoom*sin((360.0f - direction)*DEGREES),
		position.y,
		position.z + cameraZoom*cos((360.0f - direction)*DEGREES));

}

Wheel::~Wheel()
{
}
