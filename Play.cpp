#include "Play.h"
#include "Application.h"


Play::Play()
{
}

bool Play::OnEnter()
{
	INPUT->InitialiseJoysticks();
	camera = new Camera;
	skybox = new Skybox;
	world = new World;
	player = new Wheel;
	speedometer = new HUD;
	powerupHappyWheel = new Powerup;
	powerupTwister = new Powerup;

	powerupHappyWheel->Create("turbo", glm::vec3(37.0f, 0.3f, 30.0f));
	powerupTwister->Create("instantstop", glm::vec3(38.0f, 0.3f, 30.0f));

	Shader::Start();

	m_object.push_back(camera);
	m_object.push_back(skybox);
	m_object.push_back(world);
	m_object.push_back(powerupHappyWheel);
	m_object.push_back(powerupTwister);
	m_object.push_back(player);
	m_object.push_back(speedometer);
	for (unsigned i = 0; i < m_object.size(); i++)
	{
		m_object[i]->Initialize();
	}

	speedometer->SetSpeed(&player->Velocity());
	speedometer->SetTimer(&player->StartTimer());
	speedometer->SetGear(&player->Gear());

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return true;
}

void Play::Update()
{
	camera->Target() = player->CameraAt();
	camera->Position() = player->CameraPosition();
	for (unsigned int i = 0; i < m_object.size(); i++)
	{
		m_object[i]->Update();
	}
}

void Play::HandleEvents()
{
	for (unsigned int i = 0; i < m_object.size(); i++)
	{
		m_object[i]->HandleEvents();
	}

	if (PRESSING(SDL_SCANCODE_ESCAPE))			Application::Instance()->Quit();

	if (PRESSING(SDL_SCANCODE_1)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);
	}
	if (PRESSING(SDL_SCANCODE_2)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
	}
}

void Play::Render()
{
	glClearColor(0.4f, 0.4f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Shader::Push(Shader::Identity());
	{
		for (int i = 0; i != m_object.size(); i++)
		{
			m_object[i]->Render();
		};
	}
	Shader::Pop();
}

bool Play::OnExit()
{
	return true;
}

Play::~Play()
{
	delete camera;
	delete skybox;
	delete world;
	delete player;
}
