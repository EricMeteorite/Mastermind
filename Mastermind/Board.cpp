#include "Board.h"
#include "Game.h"
#include "Player.h"
#include <iostream> //Included for the basic IO functionality.
#include <string> //Included for string functions.
#include <cstdlib> //Included for rand().
#include <ctime> //Included to allow the program to have more true randomness.

using namespace std;
///////////////////////// Board CLASSS ////////////////////////////////////////////

Board::Board() {
	code = "0B0W";

} //Default constructor (for the result).

Board::~Board() {

}

Board::Board(string& userCode) {
	code = userCode;

} 

void Board::set(string& userCode)
{
	//Sets the code to the passed string.
	code = userCode;
}

string Board::value()
{
	//Returns the value of the code.
	return code;
}

//////////////////////////////////////////////////////////////////////////////////
