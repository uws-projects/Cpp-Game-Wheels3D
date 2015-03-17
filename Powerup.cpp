#include "Powerup.h"

Powerup::Powerup()
{
}

void Powerup::Initialize()
{
	//Initialize the power-up object
	std::cout << "\nPower-ups Intialized";
	model = Load::Obj("model/cubeModel.obj");
	indexCount = Load::meshCount();
	scale = glm::vec3(0.18f);
}

void Powerup::Create(char *type, glm::vec3 PowerPosition)
{
	//Inputs for the choice of type of powerup and position in world.
	position = PowerPosition;
	if (type == "fog"){ texture = Load::PNG("model/powerups/fog.png"); }
	if (type == "happywheel"){ texture = Load::PNG("model/powerups/happy_wheel.png"); }
	if (type == "instantstop"){ texture = Load::PNG("model/powerups/instant_stop.png"); }
	if (type == "jump"){ texture = Load::PNG("model/powerups/jump.png"); }
	if (type == "oildrop"){ texture = Load::PNG("model/powerups/oil_drop.png"); }
	if (type == "repair"){ texture = Load::PNG("model/powerups/repair.png"); }
	if (type == "reversecontrols"){ texture = Load::PNG("model/powerups/reverse_controls.png"); }
	if (type == "shield"){ texture = Load::PNG("model/powerups/shield.png"); }
	if (type == "spikestrip"){ texture = Load::PNG("model/powerups/spikestrip.png"); }
	if (type == "turbo"){ texture = Load::PNG("model/powerups/turbo.png"); }
	if (type == "twister"){ texture = Load::PNG("model/powerups/twister.png"); }
}

// Effects to be applied to wheel upon collision
void Powerup::ApplyFog()
{
	//Apply effect
}

void Powerup::ApplyHappyWheel()
{
	//Apply effect
}

void Powerup::ApplyInstantStop()
{
	//Apply effect
}

void Powerup::ApplyJump()
{
	//Apply effect
}

void Powerup::ApplyOilDrop()
{
	//Apply effect
}

void Powerup::ApplyRepair()
{
	//Apply effect
}

void Powerup::ApplyReverseControls()
{
	//Apply effect
}

void Powerup::ApplyShield()
{
	//Apply effect
}

void Powerup::ApplySpikeStrip()
{
	//Apply effect
}

void Powerup::ApplyTurbo()
{
	//Apply effect
}

void Powerup::ApplyTwister()
{
	//Apply effect
}

void Powerup::Update()
{
	//Detect collision (centre of powerup expanding evenly outwards) then apply effect
}


void Powerup::Render()
{
	//Render the powerup
	Shader::Push();
	{
		Shader::Top() = glm::translate(Shader::Top(), position);
		Shader::Top() = glm::scale(Shader::Top(), scale);
		Shader::Top() = glm::rotate(Shader::Top(), rotationSpeed, glm::vec3(1.0f, 1.0f, 1.0f));
		Shader::AddMaterial(material);
		Shader::SetUniform("ModelViewMatrix", Shader::Top());
		Shader::SetUniform("MVP", Shader::ProjectionMatrix()*Shader::Top());
		Shader::Bind(0, "tex", texture);
		glBindVertexArray(model);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	rotationSpeed += 2.0f; //Power-up rotation speed
	Shader::Pop();

}

void Powerup::HandleEvents()
{
}

Powerup::~Powerup()
{
}
