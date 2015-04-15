#pragma once
#include "Wheel.h"
#include "Track.h"
#include "Application.h"

#ifndef COMPLETE
#define COMPLETE true
#endif

#ifndef INCOMPELTE
#define INCOMPLETE false
#endif

Wheel::Wheel()
{
}

bool raceStarted = false;

// better texture wrong shape roata obj, cull-face back, texture texture.bmp

void Wheel::Initialize()
{
	model = Load::Obj(".\\model\\wheels_ultimul.obj");
	//model = Load::Obj(".\\model\\world\\roata.obj");
	indexCount = Load::meshCount();
	material.Ka = glm::vec4(0.0, 0.0, 0.0, 1.0);
	material.Kd = glm::vec4(0.649020f, 0.837255f, 0.717647f, 1.0);
	material.Ks = glm::vec4(0.33, 0.33, 0.33, 1.0);
	material.shininess = 0.5f;

	light.position = glm::vec4(position, 1.0f);

	
	direction = 0;
	rotationAngle = 0;
	Acceleration = 0;
	rubber = Load::PNG(".\\model\\world\\wheel\\texture.png");
	scale = glm::vec3(1.0f);
	position = glm::vec3(37.2421f, RADIUS - tyreAspectRatio, 19.7211f-RADIUS);

	damage = 0.0f;
	detachedCamera = false;
	canControl = false;
	canRace = false;

	cameraWidthX = 0.0f;
	cameraWidthZ = 0.0f;
	cameraHeight = position.y*2.5f*80;
	cameraZoom = 2.0f;
	hugeZoom = false;
	zoomAmount = 0.01f;

	raceTrack = new Track(this);
	raceTrack->Initialize();
	raceComplete = false;
	finalTime = 0.0f;

	initializePhysics();

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
		Shader::SetUniform("attConst", 1.0f);
		Shader::SetUniform("attLinear", 0.0f);
		Shader::SetUniform("attQuadratic", 0.0f);
		// TO DO
		// when texture coordinates will be created bind this
		Shader::Bind(0, "tex", rubber);
		glBindVertexArray(model);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glDisable(GL_CULL_FACE);
	}
	Shader::Pop();
}

glm::vec3 moveForward(glm::vec3 pos, GLfloat angle, GLfloat d, GLfloat rotation) {
	return glm::vec3(pos.x + d*std::sin(angle*DEGREES), pos.y + d*std::sin(rotation*DEGREES), pos.z - d*std::cos(angle*DEGREES));
}

void Wheel::gearUp()
{
	if (currentGear == REVERSE)
		currentGear = NEUTRAL;
	else if (currentGear < 6) currentGear++;
}

void Wheel::gearDown()
{
	if (currentGear != REVERSE) {
		if (currentGear == NEUTRAL)
			currentGear = REVERSE;
		else if (currentGear > 0) currentGear--;
	}
}

void Wheel::Update()
{
	// comment next line to disable collisions
	raceTrack->Update();

	// gearbox automatic down shifting
	if (currentGear == 6 && velocity < 17) currentGear = 5;
	if (currentGear == 5 && velocity < 14) currentGear = 4;
	if (currentGear == 4 && velocity < 10) currentGear = 3;
	if (currentGear == 3 && velocity < 6) currentGear = 2;
	if (currentGear == 2 && velocity < 3) currentGear = 1;

	// gearbox automatic up shifting if enabled
	if (automaticGearBox)
	{
		if (currentGear == 1 && velocity > 3) currentGear = 2;
		if (currentGear == 2 && velocity > 6) currentGear = 3;
		if (currentGear == 3 && velocity > 10) currentGear = 4;
		if (currentGear == 4 && velocity > 14) currentGear = 5;
		if (currentGear == 5 && velocity > 17) currentGear = 6;

	}

	// normal camera positioning
	if (!detachedCamera)
	{
		cameraPosition = moveForward(position, direction, 1.0f, cameraTurn);
		cameraPosition.y = cameraHeight;
		cameraPosition.x += cameraWidthX;
		cameraPosition.z += cameraWidthZ;
	}
	else
	{	
		canControl = false;
		cameraHeight += 0.1f;
		cameraPosition.y = cameraHeight;
		if (cameraHeight > 15.0f)
			Application::Instance()->Quit();
	}
	
	cameraAt = glm::vec3(position.x + cameraZoom*sin((360.0f - direction)*DEGREES), position.y, position.z + cameraZoom*cos((360.0f - direction)*DEGREES));

	// camera repositioning on collision
	
	if (raceTrack->ContactLeftX()) { if (cameraWidthX > -CAMERAMAXIMUMDISPLACEMENT) cameraWidthX -= CAMERACOLLISIONSPEED; }
	else if (cameraWidthX < 0) cameraWidthX += CAMERACOLLISIONSPEED;
	if (raceTrack->ContactRightX()) { if (cameraWidthX < CAMERAMAXIMUMDISPLACEMENT) cameraWidthX += CAMERACOLLISIONSPEED; }
	else if (cameraWidthX > 0) cameraWidthX -= CAMERACOLLISIONSPEED;
	if (raceTrack->ContactLeftZ()) { if (cameraWidthZ > -CAMERAMAXIMUMDISPLACEMENT) cameraWidthZ -= CAMERACOLLISIONSPEED; }
	else if (cameraWidthZ < 0) cameraWidthZ += CAMERACOLLISIONSPEED;
	if (raceTrack->ContactRightZ()) { if (cameraWidthZ < CAMERAMAXIMUMDISPLACEMENT) cameraWidthZ += CAMERACOLLISIONSPEED; }
	else if (cameraWidthZ > 0) cameraWidthZ -= CAMERACOLLISIONSPEED;
	

}

void Wheel::initializePhysics()
{
	// Set gearbox to manual or automatic; 
	automaticGearBox = true;
	// Set in neutral
	currentGear = NEUTRAL;
	// Reset forces and speed
	velocity = 0.0f;
	Acceleration = 0.0f;
	F_traction = 0.0f;
	F_long = 0.0f;
	F_drag = 0.0f;
	F_rr = 0.0f;
	F_braking = 0.0f;
	// Set direction forward
	u = 1.0f;
	// Initialize Engine Power and joystick trigger value
	EnginePower = 0;
	Trigger = 0.0f; 

	// Set gear ratios
	gears[0] = 0.00f;
	gears[1] = 0.50f;
	gears[2] = 0.80f;
	gears[3] = 1.05f;
	gears[4] = 1.46f;
	gears[5] = 1.70f;
	gears[6] = 1.82f;
	gears[7] = 0.50f;
	canGearDown = true;
	canGearUp = false;
}

void Wheel::updatePhysics()
{
	// TO DO: cleanup and tweak
	if (rotationAngle > 360) rotationAngle -= 360;

	// clamp direction between 0 and 360 degrees
	if (direction > 360) direction -= 360;
	if (direction < -360) direction += 360;

	// calculate resistance forces
	F_drag = -Cdrag * 0.5f * AREA * RHOAIR * velocity;
	F_rr = -Crolling * velocity;
	float revolutions = velocity / 2 * PI;
	if (abs(F_long) < 0.4f) F_long = 0.0f;

	Acceleration = F_long / MASS;
	velocity = velocity + Acceleration * DT * boostTurbo;

	// if the velocity is too close to 0, make it 0
	if (abs(velocity) < 0.082) {
		velocity = 0.0f; 
	}

	switch (currentGear)
	{
	case 7:	// reverse case
	{
				u = -1;
				if (EnginePower < 0)
				{
					int brake = abs(EnginePower);
					if (brake > 90) { revolutions = 0; }
					if (abs(velocity) > 0)
						F_braking = -u * Cbraking;
					F_long = F_braking + F_drag + F_rr;
				}
				else
				{
					F_traction = u * EnginePower * gears[currentGear] * 0.01f * (100 - damage);
					F_long = F_traction + F_drag + F_rr;
				}
				SOUND->Play(ENGINELOOP, EngineVolume, (44100 + abs(velocity) * 3000.0f));
	}
		break;
	case 0:	// neutral case
	{
				if (EnginePower < 0)
				{
					int brake = abs(EnginePower);
					if (brake > 90) { revolutions = 0; }
					if (abs(velocity) > 0)
						F_braking = -u * Cbraking;
					F_long = F_braking + F_drag + F_rr;
				}
				else
				{
					F_long = F_drag + F_rr;	
				}
				SOUND->Play(ENGINELOOP, EngineVolume, 44100 + 800.0f * abs(EnginePower));
	}
		break;
	default: // regular gear
	{
				 u = 1;
				 if (EnginePower < 0)
				 {
					 int brake = abs(EnginePower);
					 if (brake > 90) { revolutions = 0; }
					 if (abs(velocity) > 0)
						 F_braking = -u * Cbraking;
					 F_long = F_braking + F_drag + F_rr;
				 }
				 else
				 {
					 F_traction = u * EnginePower * gears[currentGear] * 0.01f * (100 - damage);
					 F_long = F_traction + F_drag + F_rr;
				 }
				 // play sound at volume 0.01, with a freq multiplier of 2000 
				 SOUND->Play(ENGINELOOP, EngineVolume, (44100 + velocity * 2000.0f));
	}
		break;
	}

	rotationAngle += revolutions;

	// report section, uncomment as necessary
	//std::cout << "\nF_long: " << F_long;
	//std::cout << "\nAcceleration : " << Acceleration;
	//std::cout << "\nF_traction: " << F_traction;
	//std::cout << "\nEngine Power: " << EnginePower;
	//std::cout << "\nGear: " << currentGear << " velocity : " << velocity;
}

void Wheel::HandleEvents()
{

	if (!raceStarted)
	{
		if (cameraHeight > position.y * 2.5f)
		{
			cameraHeight -= 0.3f;
			float test = (200.0f - cameraHeight) / 10000.0f;
			if (test > EngineVolume) test = EngineVolume;
			std::cout << "Sound volume: " << test << "\n";
			SOUND->Play(ENGINELOOP, test, 44100 + 700.0f * abs(EnginePower));
			canGearUp = false;
		}
		else
		{
			raceStarted = true;
			canControl = true;
		}
		if (raceStarted && !raceComplete) {
			startTime = SDL_GetTicks() + 3000;	
		}
	}
	else
	{
		if (SDL_GetTicks() - startTime > 0) canRace = true;
		if (JOY->JoysticksInitialised())
		{

			float RightStickXvalue = (JOY->xValue(JOYSTICK1, RIGHTSTICK)) / 364 + 15.23123f;
			float RightStickYvalue = (JOY->yValue(JOYSTICK1, RIGHTSTICK)) / 364 + 15.23123f;
			float LeftStickXValue = (JOY->xValue(JOYSTICK1, LEFTSTICK)) / 364 + 15.23123f;

			// record trigger values and invert axis 
			/* by default the right trigger returns negative values
			*  and left trigger returns positive values, but we want to
			*  use the commands inverted (RT gas LT brake) so we multiply by -1
			*/
			Trigger = JOY->zValue(JOYSTICK1)*-1;

			// transform joystick axis in percentage values from -100 to 100
			EnginePower = (Trigger / 32765) * 100;

			// value to reduce turning at high speeds

			float toAdd = (LeftStickXValue / 50.0f) * boostTurn;

			if (LeftStickXValue > 17.0f || LeftStickXValue < -17.0f)
			{
				if (reverseControls) direction -= toAdd;
				else direction += toAdd;
			}

			// gear up on release of RB
			if (canGearUp && canRace)
			{
				if (JOY_RB) {
					canGearUp = false;
					if (reverseControls) gearDown(); else gearUp();
				}
			}

			// gear down on release of LB
			if (canGearDown)
			{
				if (JOY_LB) {
					canGearDown = false;
					if (reverseControls) gearUp(); else gearDown();
				}
			}

			// update toggles
			if (JOY_RB == SDL_RELEASED) canGearUp = true;
			if (JOY_LB == SDL_RELEASED) canGearDown = true;
	
			// pressing start enables fast zoom
			if (JOY_START)
			{
				hugeZoom = !hugeZoom;
				SDL_Delay(50);
				if (hugeZoom) zoomAmount = 1.0f;
				else zoomAmount = 0.1f;
			}

			{
				if (RightStickYvalue > 10) if (cameraHeight > 0.1f) {
					cameraHeight -= zoomAmount;
				}
				if (RightStickYvalue < -10) {
					cameraHeight += zoomAmount;
				}
				if (RightStickXvalue > 10) if (cameraZoom < 10.0f) {
					cameraZoom += zoomAmount;
				}
				if (RightStickXvalue< -10) if (cameraZoom > 0.1f) {
					cameraZoom -= zoomAmount;
				}
			}
			cameraTurn = -RightStickYvalue;
		}
		else
		{
			// movement
			if (PRESSING(SDL_SCANCODE_W)) if (EnginePower < 100)  EnginePower += 2;
			if (PRESSING(SDL_SCANCODE_S)) if (EnginePower > -100) EnginePower -= 2;
			if (PRESSING(SDL_SCANCODE_D)) direction += 2;
			if (PRESSING(SDL_SCANCODE_A)) direction -= 2;

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
		}

		updatePhysics();
		// update wheel position
		position.x += velocity * -std::sin(direction*DEGREES) * DT;
		position.z += velocity * std::cos(direction*DEGREES) * DT;
	}
}

Wheel::~Wheel()
{
}
