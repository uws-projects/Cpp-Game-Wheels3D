#ifndef __Highscore__
#define __Highscore__

#include "Highscore.h"

Highscore * Highscore::s_pInstance = 0;
highscore Highscore::player[10] = {};
highscore Highscore::temp = {};
string Highscore::data[20] = {};
GLuint Highscore::playerTexture[10] = {};
GLuint Highscore::scoreTexture[10] = {};

string Highscore::miliToDigit(int startTime)
{
	int minutes = (int)startTime / 60000;
	int seconds = (int)(startTime - (minutes * 60000)) / 1000;
	int mili = (int)(startTime - minutes * 60000 - seconds * 1000);
	std::string result = "";
	if (minutes < 10)
	{
		result += "0" + std::to_string(minutes) + ":";
	}
	else
	{
		result += std::to_string(minutes) + ":";
	}
	if (seconds < 10)
	{
		result += "0" + std::to_string(seconds) + ":";
	}
	else
	{
		result += std::to_string(seconds) + ":";
	}
	if (mili == 0)
	{
		result += "000";
	}
	else
	{
		result += std::to_string(mili);
	}
	return result;
}

void Highscore::Save()
{
	ofstream f;
	f.open(".\\highscore");
	
	for (int i = 0; i < 9; i++)
	{
		f << player[i].playerNames;
		f << endl << player[i].playerScore << endl;
	}
	f << player[9].playerNames;
	f << endl << player[9].playerScore;
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
		player[j].stringScore= data[i];
		player[j].playerScore = atof(player[j].stringScore.c_str());
		if (player[j].playerScore == 999999) {
			player[j].stringScore = "0:00:000";
		}
		else {
			player[j].stringScore = miliToDigit(player[j].playerScore);
		}
		i++;
		j++;
	}
}

void Highscore::AddScore(int score, string name)
{
	int i = 0, j = 9;
	while (i < 10)
	{
		if (score <= player[i].playerScore)
		{
			temp = player[i];
			player[i].playerScore = score;
			player[i].playerNames = name;
			player[i].stringScore = miliToDigit(score);
			for (; j>i; j--) {
				player[j] = player[j - 1];
			}
			player[i + 1] = temp;
			break;
		}
		else
			i++;
	};
	Save();
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
}

#endif // defined (__Highscore__)