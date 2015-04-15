#ifndef __Highscore__
#define __Highscore__

#include "Highscore.h"

Highscore * Highscore::s_pInstance = 0;
highscore Highscore::player[10] = {};
highscore Highscore::temp = {};
string Highscore::data[20] = {};
GLuint Highscore::playerTexture[10] = {};
GLuint Highscore::scoreTexture[10] = {};


void Highscore::save()
{
	fstream f;
	f.open(".\\highscore");
	
	for (int i = 0; i < 10; i++)
	{
		f << endl << player[i].playerNames;
		f << endl << player[i].playerScore;
	}
	f.close();
}

void Highscore::load()
{
	fstream f;
	int i = 0;
	f.open(".\\highscore");
	while (!f.eof())
	{
		getline(f, data[i++]);
	}
	f.close();
}

void Highscore::formatData()
{
	int i = 0, j = 0;
	while (i < 20)
	{
		player[j].playerNames = data[i++];
		player[j].playerScore = atof(player[j++].stringScore.c_str());
		i++;
	}
}

void Highscore::AddScore(float score, string name)
{
	int i = 0, j = 9;
	while (i < 10)
	{
		if (score >= player[i].playerScore)
		{
			temp = player[i];
			player[i].playerScore = score;
			player[i].playerNames = name;
			for (; j>i; j--) {
				player[j] = player[j - 1];
			}
			player[i + 1] = temp;
			break;
		}
		else
			i++;
	};
	save();
}

void Highscore::Reset()
{
	fstream f;
	int i = 0;
	f.open(".\\default");
	while (!f.eof())
	{
		getline(f, data[i++]);
	}
	f.close();
	formatData();
	save();
}

#endif // defined (__Highscore__)