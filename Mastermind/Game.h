#pragma once
#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Player.h"
#include <iostream> //Included for the basic IO functionality.
#include <string> //Included for string functions.
#include <cstdlib> 
#include <time.h> //Included to allow the program to have more true randomness.
#include <iomanip> //Included for basic IO manipulation.
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>

#define endl '\n' //Overriding "endl" with the ASCII character '\n' 

using namespace std;

class Game// A class that contains the board, the results, and the functions to play.
{
private:
	char board[10][5];
	string results[10];
	Board code, guess, result;
	vector<Player> playerList;
	vector<Player>::iterator iterPlayer;// Create a iterator to point playerList.
	//string empty; //Global empty string to be passed to constructors.
public:
	//bool checkForRepetitions(string& userCode);
	void initialiseGame();
	//--------------------------------------------
	void placeData();
	void compareGuess();
	void checkCode(string& userCode);
	void checkHiddenCode(string& userCode);
	void checkMultiInput(string& userCode);
	void inputGuess();
	void inputMultiGuess();
	void setCode();
	void inputCode(string userCode);
	//--------------------------------------------
	void gameMenu();
	void playLoop();
	bool multiPlayer(Player& playCall);
	bool singlePlayer(Player& playCall);
	void displayBoard();
	void checkScore(Player& playCall);
	void getHint();
	int calculateScore();
	string computerBox();
	//--------------------------------------------
	char askCharInput(string inputRequire);
	int askIntInput(string inputRequire);
	string askStrInput(string inputRequire);
	//--------------------------------------------
	void displayFile(string filename);
	void saveData(Player& playCall);
	void checkResult(Player& playCall);
	void delResult();
	void displayResult();
	void infoInput();
	void displayPlayerInfo(const Player& playCall);
};

#endif //GAME_H
