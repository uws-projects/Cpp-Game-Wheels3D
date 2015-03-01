#pragma once
#include "Wheel.h"

Wheel::Wheel()
{
}

void Wheel::Initialize()
{
	firstPersonCamera.Initialize();
	model = Load::Obj(".\\model\\wheels_ultimul.obj");
	//model = Load::Obj(".\\model\\world\\wheel.model");
	indexCount = Load::meshCount();
	material.Ka = glm::vec4(0.0, 0.0, 0.0, 1.0);
	material.Kd = glm::vec4(0.649020f, 0.837255f, 0.717647f, 1.0);
	material.Ks = glm::vec4(0.33, 0.33, 0.33, 1.0);
	material.shininess = 0.5f;

	light.position = glm::vec4(position, 1.0f);

	trackSection = 0;
	direction = 0;
	rotationAngle = 0;
	Acceleration = 0;
	rubber = Load::BMP(".\\model\\world\\wheel\\texture.bmp");
	scale = glm::vec3(1.0f);
	position = glm::vec3(37.2421f, RADIUS - tyreAspectRatio, 19.7211f);
	lastPosition = position;

	cameraHeight = position.y*2.5f;
	cameraZoom = 2.0f;
	editingCameraPosition = false;
	hugeZoom = false;
	zoomAmount = 0.1f;

	initializePhysics();

	glEnable(GL_CULL_FACE);
}

void Wheel::Render()
{

	Shader::Push();
	{
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_FRONT);
		Shader::Use(Shader::Bump());
		Shader::AddLight(light);
		Shader::AddMaterial(material);
		
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

void Wheel::checkTrack(int& section) {
	float velocityPenalty = 0.95f;
	switch (section)
	{
	case 0: {
				if (position.x < 35.9921f + RADIUS) { position.x = 35.9921f + RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				else { collidedRightX = false; }
				if (position.x > 38.4921f - RADIUS) { position.x = 38.4921f - RADIUS; Velocity *= velocityPenalty; collidedLeftX = true; }
				else { collidedLeftX = false; }
				if (position.z < 4.73671f) { section = 31; }
				if (position.z > 104.737f) { section = 1; }
	} break;
	case 1: {				
				if (position.z > 108.237f - RADIUS) { position.z = 108.237f - RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				else { collidedLeftZ = false; }
				if (position.x > 38.4921f - RADIUS) { position.x = 38.4921f - RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				else { collidedRightX = false; }
				if (position.z < 104.737f) { section = 0; }
				if (position.x < 35.9921f) { section = 2; }
	} break;
	case 2: {
				if (position.z < 104.737f + RADIUS) { position.z = 104.737f + RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				else { collidedRightZ = false; }
				if (position.z > 108.237f - RADIUS)	{ position.z = 108.237f - RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				else { collidedLeftZ = false; }
				if (position.x < 2.99211f) { section = 3; }
				if (position.x > 35.9921f) { section = 1; }
	} break;
	case 3: {
				if (position.z < 104.737f + RADIUS) { position.z = 104.737f + RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				else { collidedLeftZ = false; }
				if (position.x < 1.49211f + RADIUS) { position.x = 1.49211f + RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				else { collidedRightX = false; }
				if (position.z > 108.237f) { section = 4; }
				if (position.x > 2.99211f) { section = 2; }
	} break;
	case 4: {
				if (position.x > 2.99211f - RADIUS) { position.x = 2.99211f - RADIUS; Velocity *= velocityPenalty; collidedLeftX = true; }
				else { collidedLeftX = false; }
				if (position.x < 1.49211f + RADIUS) { position.x = 1.49211f + RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				else { collidedRightX = false; }
				if (position.z < 108.237f) { section = 3; }
				if (position.z > 158.237f) { section = 5; }
	} break;
	case 5: {
				if (position.x < 1.49211f + RADIUS) { position.x = 1.49211f + RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				else { collidedRightX = false; }
				if (position.z > 160.737f - RADIUS) { position.z = 160.737f - RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				else { collidedLeftZ = false; }
				if (position.x > 2.99211f) { section = 6; }
				if (position.z < 158.237f) { section = 4; }
	} break;
	case 6: {
				if (position.z > 160.737f - RADIUS) { position.z = 160.737f - RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				else { collidedLeftZ = false; }
				if (position.z < 158.237f + RADIUS) { position.z = 158.237f + RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				else { collidedRightZ = false; }
				if (position.x < 2.99211f) { section = 5; }
				if (position.x > 44.9921f) { section = 7; }
	} break;
	case 7: {
				if (position.z > 160.737f - RADIUS) { position.z = 160.737f - RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				else { collidedRightZ = false; }
				if (position.x > 49.4921f - RADIUS) { position.x = 49.4921f - RADIUS; Velocity *= velocityPenalty; collidedLeftX = true; }
				else { collidedLeftX = false; }
				if (position.x < 44.9921f) { section = 6; }
				if (position.z < 158.237f) { section = 8; }
	} break;
	case 8: {
				if (position.x > 49.4921f - RADIUS) { position.x = 49.4921f - RADIUS; Velocity *= velocityPenalty; collidedLeftX = true; }
				else { collidedLeftX = false; }
				if (position.x < 44.9921f + RADIUS) { position.x = 44.9921f + RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				else { collidedRightX = false; }
				if (position.z > 158.237f) { section = 7; }
				if (position.z < 152.237f) { section = 9; }
	} break;
	case 9: {
				if (position.x < 44.9921f + RADIUS) { position.x = 44.9921f + RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				else { collidedRightX = false; }
				if (position.z < 144.737f + RADIUS) { position.z = 144.737f + RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				else { collidedLeftZ = false; }
				if (position.x > 49.4921f) { section = 10; }
				if (position.z > 152.237f) { section = 8; }
	} break;
	case 10: {
				if (position.z < 144.737f + RADIUS) { position.z = 144.737f + RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				else { collidedRightZ = false; }
				if (position.z > 152.237f - RADIUS) { position.z = 152.237f - RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				else { collidedLeftZ = false; }
				if (position.x < 49.4921f) { section = 9; }
				if (position.x > 66.4921f) { section = 11; }
	} break;
	case 11: {
			 	 if (position.z > 152.237f - RADIUS) { position.z = 152.237f - RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				 else { collidedRightZ = false; }
				 if (position.x > 68.9921f - RADIUS) { position.x = 68.9921f - RADIUS; Velocity *= velocityPenalty; collidedLeftX = true; }
				 else { collidedLeftX = false; }
				 if (position.x < 66.4921f) { section = 10; }
				 if (position.z < 144.737f) { section = 12; }
	} break;
	case 12: {
				 if (position.x > 68.9921f - RADIUS) { position.x = 68.9921f - RADIUS; Velocity *= velocityPenalty; collidedLeftX = true; }
				 else { collidedLeftX = false; }
				 if (position.x < 66.4921f + RADIUS) { position.x = 66.4921f + RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				 else { collidedRightX = false; }
				 if (position.z > 144.737f) { section = 11; }
				 if (position.z < 111.737f) { section = 13; }
	} break;
	case 13: {
				 if (position.x < 66.4921f + RADIUS) { position.x = 66.4921f + RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				 else { collidedRightZ = false; }
				 if (position.z < 109.237f + RADIUS) { position.z = 109.237f + RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				 else { collidedLeftZ = false; }
				 if (position.z > 111.737f) { section = 12; }
				 if (position.x > 68.9921f) { section = 14; }
	} break;
	case 14: {
				 if (position.z < 109.237f + RADIUS) { position.z = 109.237f + RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				 else { collidedRightZ = false; }
				 if (position.z > 111.737f - RADIUS) { position.z = 111.737f - RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				 else { collidedLeftZ = false; }
				 if (position.x < 68.9921f) { section = 13; }
				 if (position.x > 126.992f) { section = 15; }
	} break;
	case 15: {
				 if (position.z > 111.737f - RADIUS) { position.z = 111.737f - RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				 else { collidedRightZ = false; }
				 if (position.x > 137.492f - RADIUS) { position.x = 137.492f - RADIUS; Velocity *= velocityPenalty; collidedLeftX = true; }
				 else { collidedLeftX = false; }
				 if (position.x < 126.992f) { section = 14; }
				 if (position.z < 109.237f) { section = 16; }
	} break;
	case 16: {
				 if (position.x > 137.492f - RADIUS) { position.x = 137.492f - RADIUS; Velocity *= velocityPenalty; collidedLeftX = true; }
				 else { collidedLeftX = false; }
				 if (position.x < 126.992f + RADIUS) { position.x = 126.992f + RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				 else { collidedRightX = false; }
				 if (position.z > 109.237f) { section = 15; }
				 if (position.z < 90.2367f) { section = 17; }
	} break; 
	case 17: {
				 if (position.x > 137.492f - RADIUS) { position.x = 137.492f - RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				 else { collidedRightX = false; }
				 if (position.z < 87.7367f + RADIUS) { position.z = 87.7367f + RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				 else { collidedLeftZ = false; }
				 if (position.z > 90.2367f) { section = 16; }
				 if (position.x < 126.992f) { section = 18; }
	} break;
	case 18: {
				 if (position.z < 87.7367f + RADIUS) { position.z = 87.7367f + RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				 else { collidedRightZ = false; }
				 if (position.z > 90.2367f - RADIUS) { position.z = 90.2367f - RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				 else { collidedLeftZ = false; }
				 if (position.x > 126.992f) { section = 17; }
				 if (position.x < 90.9921f) { section = 19; }
	} break;
	case 19: {
				 if (position.z > 90.2367f - RADIUS) { position.z = 90.2367f - RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				 else { collidedRightZ = false; }
				 if (position.x < 88.4921f + RADIUS) { position.x = 88.4921f + RADIUS; Velocity *= velocityPenalty; collidedLeftX = true; }
				 else { collidedLeftX = false; }
				 if (position.x > 90.9921f) { section = 18; }
				 if (position.z < 87.7367f) { section = 20; }
	} break;
	case 20: {
				 if (position.x < 88.4921f + RADIUS) { position.x = 88.4921f + RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				 else { collidedRightX = false; }
				 if (position.x > 90.9921f - RADIUS) { position.x = 90.9921f - RADIUS; Velocity *= velocityPenalty; collidedLeftX = true; }
				 else { collidedLeftX = false; }
				 if (position.z > 87.7367f) { section = 19; }
				 if (position.z < 65.7367f) { section = 21; }
	} break;
	case 21: {
				 if (position.x < 88.4921f + RADIUS) { position.x = 88.4921f + RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				 else { collidedRightX = false; }
				 if (position.z < 63.2367f + RADIUS) { position.z = 63.2367f + RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				 else { collidedLeftZ = false; }
				 if (position.z > 65.7367f) { section = 20; }
				 if (position.x > 90.9921f) { section = 22; }
	} break;
	case 22: {
				 if (position.z < 63.2367f + RADIUS) { position.z = 63.2367f + RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				 else { collidedRightZ = false; }
				 if (position.z > 65.7367f - RADIUS) { position.z = 65.7367f - RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				 else { collidedLeftZ = false; }
				 if (position.x < 90.9921f) { section = 21; }
				 if (position.x > 140.992f) { section = 23; }
	} break;
	case 23: {
				 if (position.z > 65.7367f - RADIUS) { position.z = 65.7367f - RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				 else { collidedRightZ = false; }
				 if (position.x > 142.492f - RADIUS) { position.x = 142.492f - RADIUS; Velocity *= velocityPenalty; collidedLeftX = true; }
				 else { collidedLeftX = false; }
				 if (position.x < 140.992f) { section = 22; }
				 if (position.z < 63.2367f) { section = 24; }
	} break;
	case 24: {
				 if (position.x > 142.492f - RADIUS) { position.x = 142.492f - RADIUS; Velocity *= velocityPenalty; collidedLeftX = true; }
				 else { collidedLeftX = false; }
				 if (position.x < 140.992f + RADIUS) { position.x = 140.992f + RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				 else { collidedRightX = false; }
				 if (position.z > 63.2367f) { section = 23; }
				 if (position.z < 10.2367f) { section = 25; }
	} break;
	case 25: {
				 if (position.x > 142.492f - RADIUS) { position.x = 142.492f - RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				 else { collidedRightX = false; }
				 if (position.z < 5.73671f + RADIUS) { position.z = 5.73671f + RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				 else { collidedLeftZ = false; }
				 if (position.z > 10.2367f) { section = 24; }
				 if (position.x < 140.992f) { section = 26; }
	} break;
	case 26: {
				 if (position.z < 5.73671f + RADIUS) { position.z = 5.73671f + RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				 else { collidedLeftZ = false; }
				 if (position.z > 10.2367f - RADIUS) { position.z = 10.2367f - RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				 else { collidedRightZ = false; }
				 if (position.x > 140.992f) { section = 25; }
				 if (position.x < 93.9921f) { section = 27; }
	} break;
	case 27: {
				 if (position.z > 10.2367f - RADIUS) { position.z = 10.2367f - RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				 else { collidedRightZ = false; }
				 if (position.x < 91.4921f + RADIUS) { position.x = 91.4921f + RADIUS; Velocity *= velocityPenalty; collidedLeftX = true; }
				 else { collidedLeftX = false; }
				 if (position.x > 93.9921f) { section = 26; }
				 if (position.z < 5.73671f) { section = 28; }
	} break;
	case 28: {
				 if (position.x < 91.4921f + RADIUS) { position.x = 91.4921f + RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				 else { collidedRightX = false; }
				 if (position.x > 93.9921f - RADIUS) { position.x = 93.9921f - RADIUS; Velocity *= velocityPenalty; collidedLeftX = true; }
				 else { collidedLeftX = false; }
				 if (position.z > 5.73671f) { section = 27; }
				 if (position.z < 4.73671f) { section = 29; }
	} break;
	case 29: {
				 if (position.x > 93.9921f - RADIUS) { position.x = 93.9921f - RADIUS; Velocity *= velocityPenalty; collidedRightX = true; }
				 else { collidedRightZ = false; }
				 if (position.z < 2.23671f + RADIUS) { position.z = 2.23671f + RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				 else { collidedLeftZ = false; }
				 if (position.z > 4.73671f) { section = 28; }
				 if (position.x < 91.4921f) { section = 30; }
	} break;
	case 30: {
				 if (position.z < 2.23671f + RADIUS) { position.z = 2.23671f + RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				 else { collidedRightZ = false; }
				 if (position.z > 4.73671f - RADIUS) { position.z = 4.73671f - RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				 else { collidedLeftZ = false; }
				 if (position.x > 91.4921f) { section = 29; }
				 if (position.x < 38.4921f) { section = 31; }
	} break;
	case 31: {
				 if (position.z < 2.23671f + RADIUS) { position.z = 2.23671f + RADIUS; Velocity *= velocityPenalty; collidedRightZ = true; }
				 else { collidedRightZ = false; }
				 if (position.x < 35.9921f + RADIUS) { position.x = 35.9921f + RADIUS; Velocity *= velocityPenalty; collidedLeftZ = true; }
				 else { collidedLeftZ = false; }
				 if (position.x > 38.4921f) { section = 30; }
				 if (position.z > 4.73671f) { section = 0; }
	} break;
	default: 
		break;
	}
}

void Wheel::Update()
{
	// comment next line to disable collisions
	checkTrack(trackSection);
	if (currentGear == 6 && Velocity < 17) currentGear = 5;
	if (currentGear == 5 && Velocity < 14) currentGear = 4;
	if (currentGear == 4 && Velocity < 11) currentGear = 3;
	if (currentGear == 3 && Velocity < 6) currentGear = 2;
	if (currentGear == 2 && Velocity < 3) currentGear = 1;

	cameraPosition = moveForward(position, direction, 1.0f, cameraTurn);
	cameraPosition.y = cameraHeight;
	cameraPosition.x += cameraWidthX;
	cameraPosition.z += cameraWidthZ;
	cameraAt = glm::vec3(position.x + cameraZoom*sin((360.0f - direction)*DEGREES), position.y, position.z + cameraZoom*cos((360.0f - direction)*DEGREES));

	if (collidedLeftX == true) cameraCollision(1, 1);
	if (collidedRightX == true) cameraCollision(1, 2);
	if (collidedLeftZ == true) cameraCollision(2, 1);
	if (collidedRightZ == true) cameraCollision(2, 2); 
}

void Wheel::cameraCollision(int axis, int polarity){
	if (axis == 1){
		if (polarity == 1){
			cameraWidthX -= 0.01;
		}
		if (polarity == 2){
			cameraWidthX += 0.01;
		}
	}
	if (axis == 2){
		if (polarity == 1){
			cameraWidthZ -= 0.01;

		}
		if (polarity == 2){
			cameraWidthZ += 0.01;
		}
	}
}

void Wheel::initializePhysics()
{
	currentGear = NEUTRAL;
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

	gears[0] = 0.00f;
	gears[1] = 0.50f;
	gears[2] = 0.80f;
	gears[3] = 1.05f;
	gears[4] = 1.46f;
	gears[5] = 1.70f;
	gears[6] = 1.82f;
	gears[7] = 0.50f;
	canGearDown = true;
	canGearUp = true;
}

void Wheel::updatePhysics()
{
	// TO DO: cleanup and tweak
	if (rotationAngle > 360) rotationAngle -= 360;

	// clamp direction between 0 and 360 degrees
	if (direction > 360) direction -= 360;
	if (direction < -360) direction += 360;

	// calculate resistance forces
	F_drag = -Cdrag * 0.5f * AREA * RHOAIR * Velocity;
	F_rr = -Crolling * Velocity;
	float F_resistance = F_drag + F_rr;
	float revolutions = Velocity / 2 * PI;
	if (abs(F_long) < 0.4f) F_long = 0.0f;

	Acceleration = F_long / MASS;
	Velocity = Velocity + Acceleration * DT;

	// if the velocity is too close to 0, make it 0
	if (abs(Velocity) < 0.082) {
		Velocity = 0.0f; 
	}

	if (currentGear == REVERSE) {
		u = -1;
		if (EnginePower < 0)
		{
			int brake = abs(EnginePower);
			if (brake > 80) { revolutions = 0; }
			F_braking = -u * Cbraking * brake;
			F_long = F_braking + F_resistance;
		}
		else
			F_traction = u * EnginePower * gears[currentGear];
	}
	else {
		u = 1;
	}
	if (currentGear != NEUTRAL) {
		// calculate longitudinal force when braking or not
		if (EnginePower < 0)
		{
			int brake = abs(EnginePower);
			if (brake > 80) { revolutions = 0; }
			F_braking = -u * Cbraking * brake;
			F_long = F_braking + F_resistance;
		}
		else
		{
			// calculate traction force
			F_traction = u * EnginePower * gears[currentGear];

			F_long = F_traction + F_resistance;
		}
	}
	rotationAngle += revolutions;

	// report section, uncomment as necessary
	//std::cout << "\nF_long: " << F_long;
	//std::cout << "\nAcceleration : " << Acceleration;
	//std::cout << "\nF_traction: " << F_traction;
	//std::cout << "\nEngine Power: " << EnginePower;
	//std::cout << "\nGear: " << currentGear << " Velocity : " << Velocity;
}

void Wheel::HandleEvents()
{
	lastPosition = position;
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
		float toAdd = (LeftStickXValue / 50.0f);// *(1.1f + Velocity*0.8f);

		if (LeftStickXValue > 15.0f || LeftStickXValue < -15.0f) direction += toAdd;
		
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
			SDL_Delay(50);
		}

		// pressing start enables fast zoom
		if (JOY_START)
		{
			hugeZoom = !hugeZoom;
			SDL_Delay(50);
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
		if (PRESSING(SDL_SCANCODE_D)) direction+=2; 
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
	position.x += Velocity * -std::sin(direction*DEGREES) * DT;
	position.z += Velocity * std::cos(direction*DEGREES) * DT;
}

Wheel::~Wheel()
{
}
