#pragma once
#ifndef PLAYER_H
#define PLATER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <time.h>
#include <algorithm>

using namespace std;

class Player
{
private:
	string playerName;
	int playerCurrentScore;
public:
	Player(string name);
	~Player();
	string getName() const;
	int getCurrentScore() const;
	void updateScore(int tempScore);

};


#endif // !PLAYER_H
