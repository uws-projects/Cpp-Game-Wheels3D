#include "Logo.h"
#include "Application.h"
#include "Play.h"

#define DELAY 5000

bool Logo::OnEnter()
{
	Shader::Start();
	INPUT->InitialiseJoysticks();

	transparency = -0.05f;
	texture = Load::PNG(".\\model\\background.png");

	// create rectangle to cover screen based on one scalable variable
	float height = 0.66f;
	float aspect = WIDTH / HEIGHT;
	float width = height * aspect;
	GLfloat rectangle[] =
	{
		// one of the two triangles
		- width, -height, -1,
		width, height, -1,
		width, -height, -1,
		// the other triangle
		- width, -height, -1,
		-width, height, -1,
		width, height, -1
	};

	GLfloat coordinates[] =
	{
		0, 1, 1, 0, 1, 1,
		0, 1, 0, 0, 1, 0
	};

	glGenVertexArrays(1, &model);
	glBindVertexArray(model);
	{
		GLuint vertexPosition;
		glGenBuffers(1, &vertexPosition);
		glBindBuffer(GL_ARRAY_BUFFER, vertexPosition);
		glBufferData(GL_ARRAY_BUFFER, 3 * 6 * sizeof(GLfloat), rectangle, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		GLuint vertexTexCoord;
		glGenBuffers(1, &vertexTexCoord);
		glBindBuffer(GL_ARRAY_BUFFER, vertexTexCoord);
		glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(GLfloat), coordinates, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
	}
	glBindVertexArray(0);
	
	initialTime = SDL_GetTicks();
	return true;
 }

void Logo::Update()
{
	totalTime = SDL_GetTicks() - initialTime;

	// update the total time that past since beginning 
	if (totalTime > DELAY)		// if 5 seconds passed go to menu
	{
		Application::Instance()->GetStateMachine()->PushState(new Play());
	}
	if (transparency < 1.0f) {
		transparency += (float) (totalTime / 300000.0f);	// if it`s transparent make it visible
		std::cout << "\n totalTime: " << totalTime << " result: " <<  (float) totalTime / 300000.0f;
		std::cout << "\n transparency: " << transparency;
	}
	else transparency = 1.0f;		// 
	Shader::SetUniform("alfa", transparency);
}

void Logo::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	Shader::Use(Shader::ScreenHUD());
	Shader::Push(Shader::Identity());
	{
		Shader::Bind(0, "tex", texture);
		Shader::SetUniform("ProjectionMatrix", Shader::ProjectionMatrix());
		Shader::SetUniform("ModelViewMatrix", Shader::Identity());
		glBindVertexArray(model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
	Shader::Pop();
}

void Logo::HandleEvents()
{
	// if we have joystick
	// we can skip to next state by pressing either escape or start button on gamepad
	if (JOY->JoysticksInitialised()) 
	{
		if (JOY_START || PRESSING(SDL_SCANCODE_ESCAPE))
		{
			SDL_Delay(200);
			Application::Instance()->GetStateMachine()->PushState(new Play());
		}	
	}
	else
	// else we can skip it only by pressing escape
	{
		if (PRESSING(SDL_SCANCODE_ESCAPE))
		{
			SDL_Delay(200);
			Application::Instance()->GetStateMachine()->PushState(new Play());
		}
	}

}

