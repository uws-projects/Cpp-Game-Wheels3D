#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "settings.h"
using namespace std;

#define SCORE Highscore::Instance()

struct highscore {
	string playerNames;
	string stringScore;
	float playerScore;
};

class Highscore
{

public:
	static Highscore* Instance()
	{
		if (s_pInstance == 0)
		{
			s_pInstance = new Highscore();
			load();
			formatData();
			for (int i = 0; i < 10; i++) {
				playerTexture[i] = Load::Text(player[i].playerNames.c_str());
				scoreTexture[i] = Load::Text(std::to_string(player[i].playerScore).c_str());
			}
			return s_pInstance;
		}
		return s_pInstance;
	}
	~Highscore() {}
	GLuint GetPlayerTexture(int id) { return playerTexture[id]; }
	GLuint GetScoreTexture(int id) { return scoreTexture[id]; }
	string GetPlayer(int id) { return player[id].playerNames; };
	float GetScore(int id) { return player[id].playerScore; }
	void Reset();
	void AddScore(float score, string name);

private:
	void save();
	static void load();
	static void formatData();
	static highscore player[10], temp;
	static string data[20];
	static Highscore* s_pInstance;
	static GLuint playerTexture[10];
	static GLuint scoreTexture[10];
};

