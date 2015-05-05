#include "Powerup.h"
#include "settings.h"

void Powerup::Initialize()
{
	//Initialize the power-up object
	model = Load::Obj("model/cubeModel.obj");
	indexCount = Load::meshCount();
	scale = glm::vec3(0.18f);
	visible = true;
	timer = 0;
	alarm = 0;

}

Powerup::Powerup(Wheel * w, int powerType, glm::vec3 Position, int (&tex)[7])
{
	wheel = w;
	type = powerType;
	position = Position;

	texture[SHIELD] = tex[SHIELD];
	texture[HAPPYWHEEL] = tex[HAPPYWHEEL];
	texture[TURBO] = tex[TURBO];
	texture[REPAIR] = tex[REPAIR];
	texture[REVERSECONTROLS] = tex[REVERSECONTROLS];
	texture[SENSIBILITY] = tex[SENSIBILITY];
	texture[INSTANTSTOP] = tex[INSTANTSTOP];
}

bool Powerup::contact()
{
	float top = position.z + scale.z;
	float bottom = position.z - scale.z;
	float left = position.x - scale.x;
	float right = position.x + scale.x;

	float px = position.x;
	float pz = position.z;
	float pr = scale.x;

	float wx = wheel->Position().x;
	float wz = wheel->Position().z;
	float wr = wheel->Radius();

	if (wx + wr<px - pr)		return false;
	if (wx - wr>px + pr)		return false;
	if (wz - wr>pz - pr)		return false;
	if (wz + wr<pz + pr)		return false;
	if (!visible)				return false;
	return true;
}

void Powerup::Update()
{
	if (contact())
	{
		visible = false;
		switch (type)
		{
		case SENSIBILITY:		wheel->ApplySensibility();		alarm = (int) VALUES->settings[Duration_Sensibility]; break;
		case HAPPYWHEEL:		wheel->ApplyHappyWheel();		alarm = (int) VALUES->settings[Duration_HappyWheel]; break;
		case TURBO:				wheel->ApplyTurbo();			alarm = (int) VALUES->settings[Duration_Turbo]; break;
		case SHIELD:			wheel->ApplyShield();			alarm = (int) VALUES->settings[Duration_Shield]; break;
		case REVERSECONTROLS:	wheel->ApplyReverseControls();	alarm = (int) VALUES->settings[Duration_Reverse_Controls]; break;
		case REPAIR:			wheel->ApplyRepair();			alarm = 10000; break;
		case INSTANTSTOP:		wheel->ApplyInstantStop();		alarm = 10000; break;
			
		default:
			break;
		}
		timer = SDL_GetTicks();
	}
	if (!visible)
	{
		if (SDL_GetTicks() - timer > alarm)
		{
			switch (type)
			{
			case SENSIBILITY:		wheel->UndoSensibility();		break;
			case HAPPYWHEEL:		wheel->UndoHappyWheel();		break;
			case TURBO:				wheel->UndoTurbo();				break;
			case SHIELD:			wheel->UndoShield();			break;
			case REVERSECONTROLS:	wheel->UndoReverseControls();	break;
			
			default:
				break;
			}
		}
		if (SDL_GetTicks() - timer > 10000)
		{
			type = rand() % 7;
			visible = true;
		}
	}
}


void Powerup::Render()
{
	if (visible) {
		glEnable(GL_DEPTH_TEST);
		glCullFace(GL_BACK);
		//Render the powerup
		Shader::Push();
		{
			Shader::Top() = glm::translate(Shader::Top(), position);
			Shader::Top() = glm::scale(Shader::Top(), scale);
			Shader::Top() = glm::rotate(Shader::Top(), rotationSpeed, glm::vec3(1.0f, 1.0f, 1.0f));
			Shader::AddMaterial(material);
			Shader::SetUniform("ModelViewMatrix", Shader::Top());
			Shader::SetUniform("MVP", Shader::ProjectionMatrix()*Shader::Top());
			Shader::Bind(0, "tex", texture[type]);
			glBindVertexArray(model);
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
		rotationSpeed += 1.5f; //Power-up rotation speed
		Shader::Pop();
	}
}

void Powerup::HandleEvents()
{
}

Powerup::~Powerup()
{
	delete wheel;
	delete Powercube;
}
