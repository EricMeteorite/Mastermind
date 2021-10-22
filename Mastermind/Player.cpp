#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <time.h>
#include <algorithm>
#include "Player.h"

using namespace std;

///////////////////////////////////Player CLASSS ////////////////////////////////////////////
Player::Player(string name)
{
	playerName = name;
	playerCurrentScore = 0;
}

Player::~Player()
{

}

string Player::getName() const
{
	return playerName;
}

int Player::getCurrentScore() const
{
	return playerCurrentScore;
}

void Player::updateScore(int tempScore)
{
	playerCurrentScore = tempScore;
}

///////////////////////////////////////////////////////////////////////////////////////////
