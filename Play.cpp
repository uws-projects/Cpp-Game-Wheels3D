#include "Play.h"
#include "Application.h"


Play::Play()
{
}

bool Play::OnEnter()
{
	srand(SDL_GetTicks());

	camera = new Camera;
	skybox = new Skybox;
	world = new World;
	player = new Wheel;
	hud = new HUD; 
	int tex[7];
	tex[0] = Load::PNG("model/powerups/shield.png");
	tex[1] = Load::PNG("model/powerups/happy_wheel.png");
	tex[2] = Load::PNG("model/powerups/turbo.png");
	tex[3] = Load::PNG("model/powerups/repair.png");
	tex[4] = Load::PNG("model/powerups/reverse_controls.png");
	tex[5] = Load::PNG("model/powerups/sensibility.png");
	tex[6] = Load::PNG("model/powerups/instant_stop.png");	
	
	MusicSetup.length = 71000; MusicSetup.playing = false;		// 72 seconds track
	MusicCredits.length = 42000; MusicCredits.playing = false;
	MusicTitle.length = 73000; MusicTitle.playing = false;
	MusicScoreboard.length = 43000; MusicScoreboard.playing = false;

	m_object.push_back(camera);
	m_object.push_back(skybox);
	m_object.push_back(world);
	m_object.push_back(new Powerup(player, rand() % 3, glm::vec3(37.0f, 0.3f, 30.0f), tex));
	m_object.push_back(new Powerup(player, rand() % 2+4, glm::vec3(38.0f, 0.3f, 30.0f), tex));
	m_object.push_back(new Powerup(player, rand() % 2+4, glm::vec3(51.0f, 0.3f, 151.0f), tex));
	m_object.push_back(new Powerup(player, rand() % 3, glm::vec3(51.0f, 0.3f, 150.0f), tex));
	m_object.push_back(new Powerup(player, rand() % 2+4, glm::vec3(136.0f, 0.3f, 100.0f), tex));
	m_object.push_back(new Powerup(player, rand() % 3, glm::vec3(134.0f, 0.3f, 100.0f), tex));
	m_object.push_back(new Powerup(player, rand() % 7, glm::vec3(132.0f, 0.3f, 100.0f), tex));
	m_object.push_back(new Powerup(player, rand() % 2+4, glm::vec3(130.0f, 0.3f, 100.0f), tex));
	m_object.push_back(new Powerup(player, rand() % 2+4, glm::vec3(140.0f, 0.3f, 6.0f), tex));
	m_object.push_back(new Powerup(player, rand() % 7, glm::vec3(140.0f, 0.3f, 7.0f), tex));
	m_object.push_back(new Powerup(player, rand() % 3, glm::vec3(140.0f, 0.3f, 8.0f), tex));

	m_object.push_back(player);
	hud->AttachPlayer(player);
	m_object.push_back(hud);
	for (unsigned i = 0; i < m_object.size(); i++)
	{
		m_object[i]->Initialize();
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// set starting music
	SOUND->PauseMusic();
	SOUND->Music(MUSICSETUP, MusicVolume);
	MusicSetup.playing = true;
	musicTimer = SDL_GetTicks();

	return true;
}

void Play::Update()
{
	int playTime = SDL_GetTicks() - musicTimer;
	if (MusicSetup.playing) {
		if (playTime> MusicSetup.length) {
			MusicSetup.playing = false;
			SOUND->Music(MUSICTITLE, MusicVolume);
			musicTimer = SDL_GetTicks();
			playTime = SDL_GetTicks() - musicTimer;
			MusicTitle.playing = true; 
		}
	}
	if (MusicTitle.playing) {
		if (playTime > MusicTitle.length) {
			MusicTitle.playing = false;
			SOUND->Music(MUSICCREDITS, MusicVolume);
			musicTimer = SDL_GetTicks();
			playTime = SDL_GetTicks() - musicTimer;
			MusicCredits.playing = true;
		}
	}
	if (MusicCredits.playing) {
		if (playTime > MusicCredits.length) {
			MusicCredits.playing = false;
			SOUND->Music(MUSICSCOREBOARD, MusicVolume);
			musicTimer = SDL_GetTicks();
			playTime = SDL_GetTicks() - musicTimer;
			MusicScoreboard.playing = true;
		}
	}
	if (MusicScoreboard.playing) {
		if (playTime > MusicScoreboard.length) {
			MusicTitle.playing = false;
			SOUND->Music(MUSICSETUP, MusicVolume);
			musicTimer = SDL_GetTicks();
			playTime = SDL_GetTicks() - musicTimer;
			MusicSetup.playing = true;
		}
	}

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

