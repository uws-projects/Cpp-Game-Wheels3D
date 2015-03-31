#include "Menu.h"
#include "Application.h"
#include "Play.h"

#define NUMBEROFOPTIONS 5
#define HELP 0
#define NEWGAME 1
#define HIGHSCORES 2
#define SETTINGS 3
#define ABOUT 4
#define QUIT 5

bool Menu::OnEnter()
{
	option = HELP;
	texture[HELP] = Load::PNG(".\\model\\menu\\help.png");
	texture[NEWGAME] = Load::PNG(".\\model\\menu\\race.png");
	texture[HIGHSCORES] = Load::PNG(".\\model\\menu\\highscores.png");
	texture[SETTINGS] = Load::PNG(".\\model\\menu\\settings.png");
	texture[ABOUT] = Load::PNG(".\\model\\menu\\about.png");
	texture[QUIT] = Load::PNG(".\\model\\menu\\quit.png");

	float height = 0.58f;
	float aspect = WIDTH / HEIGHT;
	float width = height * aspect;
	GLfloat rectangle[] =
	{
		// one of the two triangles
		-width, -height, -1,
		width, height, -1,
		width, -height, -1,
		// the other triangle
		-width, -height, -1,
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

	return true;
}

void Menu::HandleEvents()
{
	if (JOY->JoysticksInitialised())
	{
		// when we are in the help menu, only button A works which switches to main menu
		if (option == HELP)
		{
			if (JOY_A) option = NEWGAME;
		}
		else
		// in main menu
		// if we press RB we move cursor to the right
		// if we press LB we move cursor to the left
		{
			if (JOY_RB)
			{
				if (option < NUMBEROFOPTIONS)
					option++;
				SDL_Delay(200);
			}
			if (JOY_LB)
			{
				if (option > 1)
					option--;
				SDL_Delay(200);
			}
			// if we press Y we display the help again
			if (JOY_Y) 
			{
				option = HELP;
				SDL_Delay(200);
			}
			// if we press A, we execute the selected option
			if (JOY_A)
			{
				switch (option)
				{
				case NEWGAME: Application::Instance()->GetStateMachine()->PushState(new Play());
					break;
				case HIGHSCORES: std::cout << "Launching Highscores\n";//Application::Instance()->GetStateMachine()->PushState(new Highscore());
					break;
				case SETTINGS: std::cout << "Launching Settings\n";//Application::Instance()->GetStateMachine()->PushState(new Settings());
					break;
				case ABOUT: std::cout << "Launching About\n";//Application::Instance()->GetStateMachine()->PushState(new About());
					break;
				case QUIT: Application::Instance()->Quit();
				default:
					break;
				}
			}
		}
	}
	else
	{

	}
}

void Menu::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Shader::Use(Shader::ScreenHUD());
	Shader::Push(Shader::Identity());
	{
		Shader::Bind(0, "tex", texture[option]);
		Shader::SetUniform("ProjectionMatrix", Shader::ProjectionMatrix());
		Shader::SetUniform("ModelViewMatrix", Shader::Identity());
		glBindVertexArray(model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
	Shader::Pop();
}