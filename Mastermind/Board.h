#pragma once
#ifndef BOARD_H
#define BOARD_H

#include <iostream> //Included for the basic IO functionality.
#include <string> //Included for string functions.
#include <cstdlib>
#include <ctime> //Included to allow the program to have more true randomness.
#define endl '\n' //Overriding "endl" with the ASCII character '\n' 

using namespace std;


class Board 
{
private:
	string code;
public:
	Board();
	~Board();
	explicit Board(string& userCode);
	void set(string& userCode);
	string value();
};
#endif //BOARD_H