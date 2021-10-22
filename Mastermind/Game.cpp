#include "Board.h"
#include "Game.h"
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

int gTurn = -1; //Global variable used to count the turns of the game.
int roundTime = 1; //Global variable used to count rounds of the game.
int choice = 0; //Chioce to choose single or multi player
int gameBool = 0; //To check win or lose in one turn.
int hintTimes = 0, countSame = 0; //To check hint times.
int blackCheck = 0, whiteCheck = 0; //To check which sentence computer wil say. 
string randomHintCode = "";

//GAME initialiseBoard;

using namespace std;
///////////////////////// GAME CLASSS ///////////////////////////////////////////

void Game::initialiseGame()
{
	//To initialise the game
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 4; j++) {
			//Use 2D array and initialises the board to the character 'O'.
			board[i][j] = 'O';
		}
	}
	//Initialise the variables;
	gTurn = -1;
	hintTimes = 0, countSame = 0;
	randomHintCode = "";
}

/*Game::Game(){
	
}

Game::~Game() {

}*/

void Game::gameMenu()
{
	//This is the game menu.
	initialiseGame();
	system("cls");
	//Wee need to use srand to use time.h
	srand((int)time(NULL));
	int choose = 0;
	cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "\t             _____            _                     _         _ \n";
	cout << "\t            |     | ___  ___ | |_  ___  ___  _____ |_| ___  _| |\n";
	cout << "\t            | | | || .'||_ -||  _|| -_||  _||     || ||   || . |\n";
	cout << "\t            |_|_|_||__,||___||_|  |___||_|  |_|_|_||_||_|_||___|\n";
	cout << "\n";
	cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "\t   ---------       ---------       ---------       ---------       ---------    \n";
	cout << "\t   [1]Start        [2]About         [3]Load        [4]Delete        [5]Exit     \n";
	cout << "\t   ---------       ---------       ---------       ---------       ---------    \n";
	cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	choose = askIntInput("Please choose the number to select: ");
	//Player can enter the number to choose.
	switch (choose)
	{
	case 1:
		//Start the game, call the function playLoop() to start game loop.
		playLoop();
		break;
	case 2:
		//Display the about file.
		displayFile("mastermindAbout.txt");
		break;
	case 3:
		//Display the result history, such as who is the winner.
		displayResult();
		break;
	case 4:
		//delete the result history.
		delResult();
		break;
	case 5:
		cout << "Thank you to play Mastermind!!\n";
		system("pause");
		exit(0);
		break;
	default:
		//If enter the error number.
		cout << "Invalid input! Please try again: ";
		system("pause");
		break;
	}
	return gameMenu();
}

void Game::playLoop() {
	system("cls");
	//Enter the player info.
	infoInput();
	switch (choice) //Use choice the check game mode.
	{
	case 1: //This is the single player.
	{
		//Show the story.
		displayFile("Story.txt");
		system("cls");
		//Point to the first player.
		iterPlayer = playerList.begin();//Point the first player.
		//Count the round times.
		roundTime = 1;
		while (true) {
			//Initialise the variables.
			gameBool = 0;
			int gTurn = -1;
			for (int i = 0; i < 10; i++) {
				results[i] = "";
			}
			initialiseGame();
			//Start the game, call the singleplayer function.
			singlePlayer(*iterPlayer);
			//Check the final winner.
			if (iterPlayer->getCurrentScore() >= 5000) { //If the total score is more than 5000, then this player is the winner.
				//Show the winner.
				checkResult(*iterPlayer);
				saveData(*iterPlayer);//Saved data.
				break;
			}
			else if ((iterPlayer+1)->getCurrentScore() >= 5000) { //To check computer, if the total score is more than 5000, then this player is the winner.
				checkResult(*(iterPlayer + 1));
				saveData(*(iterPlayer + 1));//Saved data.
				break;
			}
			iterPlayer++; // Point the the computer to show the computer info.
			checkScore(*iterPlayer); // Check the computer score.
			displayPlayerInfo(*iterPlayer);
			iterPlayer = playerList.begin(); //Turn to the first player.
			roundTime++;
			system("pause");
			system("cls");
		}
		system("cls");
		break;
	}
	case 2: //This is the multi player.
	{
		iterPlayer = playerList.begin();//Point the first player.
		//COunt the round times.
		roundTime = 1;
		while (true) {
			//Initialise the variables.
			gameBool = 0;
			int gTurn = -1;
			for (int i = 0; i < 10; i++) {
				results[i] = "";
			}
			initialiseGame();
			/*if (multiPlayer(*iterPlayer))//PlayGame loop for each player, return true or false.
			{

			}*/
			system("cls");
			//Start the multi games, call the multiplayer function.
			multiPlayer(*iterPlayer);
			if (iterPlayer->getCurrentScore() >= 5000) {
				checkResult(*iterPlayer);//Display the winner.
				saveData(*iterPlayer);//Saved data.
				break;
			}
			roundTime++;
		}
		system("cls");
		break;
	}
	case 3:
	{
		//Back to menu.
		return gameMenu();
	}
	default:
		cout << "Invalid input! Please try again: ";
		system("pause");
	}

	return playLoop();
}

bool Game::singlePlayer(Player& playCall) {
	string currentPlayerName = playCall.getName();//Get the player name.
	//Computer set the hidden code.
	setCode();
	bool checkBool = true;
	while (true)
	{
		//Display player info.
		displayPlayerInfo(*iterPlayer);
		//Code breaker input code.
		inputGuess();
		//Check the code and return the result.
		compareGuess();
		//Place the player guess on the board.
		placeData();
		//Show the board.
		displayBoard();
		//Show the computer sentence.
		cout << "Computer:  " << computerBox() << endl;
		if (result.value() == "4B0W") //Player win.
		{
			cout << "You Win!" << endl << "It took you " << gTurn + 1 << " trie(s)" << endl;
			gameBool = 3;
			if (playerList.begin() + 1 == iterPlayer)//If point to the last player, then return to the first player.
			{
				iterPlayer = playerList.begin();
			}
			else {
				iterPlayer++;
			}
			checkScore(*iterPlayer); //Check the loser score.
			if (playerList.begin() == iterPlayer)//If point to the first player, then return to the last player.
			{
				iterPlayer = playerList.begin() + 1;
			}
			else {
				iterPlayer--;
			}
			checkScore(*iterPlayer); //Check the winner score.
			//Display the info.
			displayPlayerInfo(*iterPlayer);
			system("pause");
			initialiseGame();
			checkBool = true;
			break;
		}

		if (gTurn > 8) //Player lose.
		{
			cout << "You Lose!" << endl << "The code was " << code.value() << endl;
			gameBool = 2;
			if (playerList.begin() + 1 == iterPlayer)//If point to the last player, then return to the first player.
			{
				iterPlayer = playerList.begin();
			}
			else {
				iterPlayer++;
			}
			checkScore(*iterPlayer); //Check the winner score.
			if (playerList.begin() == iterPlayer)//If point to the first player, then return to the last player.
			{
				iterPlayer = playerList.begin() + 1;
			}
			else {
				iterPlayer--;
			}
			checkScore(*iterPlayer); //Check the loser score.
			//Display the info.
			displayPlayerInfo(*iterPlayer);
			system("pause");
			initialiseGame();
			checkBool = false;
			break;
		}
	}
	if (checkBool == true) {
		return true;
	}
	else {
		return false;
	}
}

bool Game::multiPlayer(Player& playCall) {
	string currentPlayerName = playCall.getName();//Get the player name.
	//Display the player info.
	displayPlayerInfo(*iterPlayer);
	string customCode;
	cout <<"Enter the hidden code: ";
	//Let Code maker input the hidden code, and check the code is right or not.
	checkMultiInput(customCode);
	system("cls");
	bool checkBool = true;
	//Exchange the player in every round.
	if (playerList.begin() + 1 == iterPlayer)//If point to the last player, then return to the first player.
	{
		iterPlayer = playerList.begin();
	}
	else {
		iterPlayer++;
	}
	while (true)
	{
		displayPlayerInfo(*iterPlayer);
		//Input Code.
		inputMultiGuess();
		//Check the code and get result.
		compareGuess();
		//Place the player guess code into the board.
		placeData();
		//Show the board.
		displayBoard();

		if (result.value() == "4B0W") //If win.
		{
			cout << "You Win!" << endl << "It took you " << gTurn + 1 << " trie(s)" << endl;
			gameBool = 3;
			if (playerList.begin() + 1 == iterPlayer)//If point to the last player, then return to the first player.
			{
				iterPlayer = playerList.begin();
			}
			else {
				iterPlayer++;
			}
			checkScore(*iterPlayer); //Check loser score.
			if (playerList.begin() == iterPlayer)//If point to the first player, then return to the last player.
			{
				iterPlayer = playerList.begin() + 1;
			}
			else {
				iterPlayer--;
			}
			checkScore(*iterPlayer); //Check winner score.
			displayPlayerInfo(*iterPlayer);
			system("pause");
			initialiseGame();
			checkBool = true;
			break;
		}

		if (gTurn > 8) //If lose.
		{
			cout << "You Lose!" << endl << "The code was " << code.value() << endl;
			gameBool = 2;
			if (playerList.begin() + 1 == iterPlayer)//If point to the last player, then return to the first player.
			{
				iterPlayer = playerList.begin();
			}
			else {
				iterPlayer++;
			}
			checkScore(*iterPlayer); //Check winner score.
			if (playerList.begin() == iterPlayer)//If point to the first player, then return to the last player.
			{
				iterPlayer = playerList.begin() + 1;
			}
			else {
				iterPlayer--;
			}
			checkScore(*iterPlayer); //Check loser score.
			displayPlayerInfo(*iterPlayer);
			system("pause");
			initialiseGame();
			checkBool = false;
			break;
		}
	}
	if (checkBool == true) {
		return true;
	}
	else {
		return false;
	}
}

void Game::infoInput()
{
	cout << "Choose the mode you want to play:" << endl;
	cout << "1- Singleplayer (Story Mode) - Random-generated code" << endl;
	cout << "2- Two-Player - One player sets the code" << endl;
	cout << "3- Quit!" << endl;
	//Let player can choose game mode.
	choice = askIntInput("Please choose: ");
	int playerNum = 0;
	if (choice == 1) {
		//Single Player.
		playerNum = 1;
	}
	else if (choice == 2) {
		//Multi Player.
		playerNum = 2;
	}
	for (int i = 0; i < playerNum; i++)//Let each player to input their name
	{
		string enterPlayerName;
		//Use stringstream to enter the Player name.
		stringstream strs;
		strs << "Please enter your name ---- Player" << i + 1 << ": ";//Using StringStream, and it is easy to display player number.
		//Use askStrInput() to enter the name.
		enterPlayerName = askStrInput(strs.str());
		Player player(enterPlayerName);//Using the Player class constructer.
		playerList.push_back(player);//Put the player name into playerList.

	}
	if (playerNum == 1)//If only has one player, then add one Computer.
	{
		Player player("Computer");
		playerList.push_back(player);

	}
}

void Game::displayPlayerInfo(const Player& playCall)
{
	string currentPlayerName;
	currentPlayerName = playCall.getName();//Get the player name.

	cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "\tPlayer: " << currentPlayerName << "   Score: " <<  playCall.getCurrentScore() << "   LeftTimes: " << 9 - gTurn << "   Round: " << roundTime << "\n";
	cout << "\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

}

void Game::displayBoard()
{
	//Display the board with the respective result next to every ROW.
	system("cls");
	//Get the result value.
	results[gTurn] = result.value();
	//Show the Board and Result.
	cout << "\t                   " << "BOARD" << "\t\t                                  " << "RESULTS" << endl;
	//There are 10 times player can try.
	for (int i = 0; i < 10; i++)
	{
		//Use ASCII to display the Board.
		cout << setw(5) << i + 1 << " | \t+-+\t+-+\t+-+\t+-+ " << endl; //First line.
		cout << setw(5) << "" << " | "; //Second line.
		for (int j = 0; j < 4; j++) {
			//Display the player guess code.
			cout << "\t|" << board[i][j] << "|" << ' ';
		}
		cout << "\n" << setw(5) << "" << " | \t+-+\t+-+\t+-+\t+-+ "; //Third line.

		cout << "\t\t                           ";
		//Display the result.
		cout << results[i];
		cout << endl;
	}
}

void Game::checkScore(Player& playCall) {
	int currentPlayerScore = 0;//Reset the temporary score.
	currentPlayerScore = calculateScore();
	playCall.updateScore(currentPlayerScore + playCall.getCurrentScore());//Update the score for this round.

}

/*void Game::totalScore(Player& playCall) {
	playCall.resetScore(playCall.getTotalScore() + playCall.getCurrentScore());//Calculate the total score for this round.
	playCall.updateScore(0);
}*/ 

int Game::calculateScore() {
	int calScore = 0;
	if (gameBool == 1) { //If win, check the winner score.
		calScore = (9 - gTurn) * 100;
		calScore += 200;
		//Delete the hint times score.
		calScore = calScore - hintTimes * 200;
		if (calScore < 0) {
			calScore = 0;
		}
	}
	else if (gameBool == 2) { //Check score (Remaining turns)
		calScore += (gTurn + 1) * 100;
		calScore += 200;
		gameBool = 0;
	}
	else if (gameBool == 3) { //If lose, check winner score.
		calScore += gTurn * 100;
		gameBool = 1;
	}

	return calScore;
}

void Game::placeData()
{
	//Places the player's guess onto the board.
	for (int i = 0; i < 4; i++) {
		//4 Elements.
		board[gTurn][i] = guess.value()[i];
	}

}

void Game::compareGuess()
{
	//Set the black and white number = 0.
	int black = 0;
	int white = 0;
	string feedback;
	//Get the Guess Code and Hidden Code.
	string guessCode = guess.value();
	string hiddenCode = code.value();

	if (guessCode == hiddenCode) {
		//If these two code is same.
		feedback = "4B0W";
		//blackCheck and whiteCheck is used in computerBox().
		blackCheck = 4, whiteCheck = 0;
	}
	//If different.
	else {
		//Different situation has some different check function.
		for (int i = 0; i < 4; i++) {
			//index is the location of guessCode[i] in hiddenCode.
			unsigned long index = hiddenCode.find(guessCode[i]);
			//guessCode[i] is not equal to hiddenCode[i]
			if (guessCode[i] != hiddenCode[i]) {
				//Check if index is exist, not unlimited(npos), and guessCode[i] equals to hiddenCode[i]
				if (index != string::npos && guessCode[index] == hiddenCode[index]) {
					//Correct location and value.
					black++;
					//Let this place become 'X'.Not delete this place, just change to 'X'.
					hiddenCode[index] = 'X';
				}
				//Correct value but not correct location.
				else if (index != string::npos && guessCode[index] != hiddenCode[index]) {
					white++;
					//Put 'X' to place the code.
					hiddenCode[index] = 'X';
				}
			}
			//Also correct location and value.
			else {
				black++;
				//Put 'X' to place the code.
				hiddenCode[index] = 'X';
			}
		}
		//Get the feedback.
		feedback += to_string(black);
		feedback += 'B';
		feedback += to_string(white);
		feedback += 'W';
		//Get the blackCheck and whiteCheck.
		blackCheck = black, whiteCheck = white;
	}
	//Put the result in set function to set the result.
	result.set(feedback);
}

/*bool Game::checkForRepetitions(string& userCode)
{
	//Checks if the 4 character code to be set has any repeated colours.
	bool repeat = false;
	for (int i = 0; i < 4; i++) {
		for (int j = i + 1; j < 4; j++) {
			if (userCode[i] == userCode[j]) {
				repeat = true;
			}
		}
	}

	return !repeat;
}*/

void Game::checkCode(string& userCode)
{
	//Checks the validity of its length and the numbers in it.
	while (true)
	{
		//Set the bool array to check each elements.
		bool validNumbers[4] = {};
		bool valid = true;
		userCode = askStrInput("Enter the guessed code: ");
		//Enter the guess code.
		if (userCode == "Q" || userCode == "q") { //Quit
			char Sure;
			//Can type Y/N to make sure.
			Sure = askCharInput("Are you sure? [Y/N]: ");
			if (toupper(Sure) == 'Y') {
				//Back to menu.
				return gameMenu();
			}
			else if (toupper(Sure) == 'N') {
				//cout << " Please enter again: ";
				continue;
			}
		}
		else if (userCode == "A" || userCode == "a") { //About
			//Display the about file.
			displayFile("mastermindAbout.txt");
			continue;
		}
		else if (userCode == "H" || userCode == "h") { //Hint
			char Sure;
			//Can type Y/N to make sure.
			Sure = askCharInput("Get Hints will let you LOSE marks...Are you sure? [Y/N]: ");
			if (toupper(Sure) == 'Y') {
				//Get the hint to player.
				getHint();
				system("pause");
				continue;
			}
			else if (toupper(Sure) == 'N') {
				//cout << " Please enter again: ";
				continue;
			}
		}

		/*for (int i = 0; i < userCode.size(); i++) {
			if (userCode[i] > 96 && userCode[i] < 123) {
				userCode[i] -= 32;
			}
		}*/

		//If the size is not 4, error length.
		if (userCode.size() != 4) {
			cout << "Invalid length, Please enter again!" << endl;
			//Jump out of this loop.
			continue;
		}

		else
		{
			for (int i = 0; i < 4; i++)
			{
				//Compare each element one by one, and then if there is something out of the range, it means it does not meet the requirements
				//	and if it is meet requirements, then return true.
				if (userCode[i] == '0' || userCode[i] == '1' || userCode[i] == '2' || userCode[i] == '3' || userCode[i] == '4' || userCode[i] == '5' || userCode[i] == '6' || userCode[i] == '7') {
					//If it is meet requirements, then return true.
					validNumbers[i] = true;
				}
			}
			//If it is not meet requirements, then return false.
			for (int i = 0; i < 4; i++) {
				//If exist error element.
				if (!validNumbers[i]) {
					//Sign this elements(numbers).
					valid = false;
				}
			}
			//Error elements(number).
			if (!valid) {
				cout << "Invalid numbers, Please enter again!" << endl;
			}
			else {
				break;
			}
		}
	}
}

void Game::checkHiddenCode(string& userCode) {
	//Takes a string s, lets the user input its value, and then checks the validity of its length and the colours in it.
	while (true)
	{
		//Set the bool array to check each elements.
		bool validNumbers[4] = {};
		bool valid = true;
		cin >> userCode;
		//Enter the guess code.
		if (userCode == "Q" || userCode == "q") {
			char Sure;
			//Can type Y/N to make sure.
			Sure = askCharInput("Are you sure? [Y/N]: ");
			if (toupper(Sure) == 'Y') {
				//Back to menu.
				return gameMenu();
			}
			else if (toupper(Sure) == 'N') {
				cout << " Please enter again: ";
				continue;
			}
		}
		else if (userCode == "A" || userCode == "a") {
			//Display the about file.
			displayFile("mastermindAbout.txt");
			continue;
		}
		else if (userCode == "H" || userCode == "h") {
			char Sure;
			//Can type Y/N to make sure.
			Sure = askCharInput("Get Hints will let you LOSE marks...Are you sure? [Y/N]: ");
			if (toupper(Sure) == 'Y') {
				//Get the hint to player.
				getHint();
				system("pause");
				cout << "Enter the guessed code: ";
				continue;
			}
			else if (toupper(Sure) == 'N') {
				cout << " Please enter again: ";
				continue;
			}
		}

		/*for (int i = 0; i < userCode.size(); i++) {
			if (userCode[i] > 96 && userCode[i] < 123) {
				userCode[i] -= 32;
			}
		}*/

		//If the size is not 4, error length.
		if (userCode.size() != 4) {
			cout << "Invalid length, Please enter again: " << endl;
			//Jump out of this loop.
			continue;
		}

		else
		{
			for (int i = 0; i < 4; i++)
			{
				//Compare each element one by one, and then if there is something out of the range, it means it does not meet the requirements
				//	and if it is meet requirements, then return true.
				if (userCode[i] == '0' || userCode[i] == '1' || userCode[i] == '2' || userCode[i] == '3' || userCode[i] == '4' || userCode[i] == '5' || userCode[i] == '6' || userCode[i] == '7') {
					//If it is meet requirements, then return true.
					validNumbers[i] = true;
				}
			}
			//If it is not meet requirements, then return false.
			for (int i = 0; i < 4; i++) {
				//If exist error element.
				if (!validNumbers[i]) {
					//Sign these elements(numbers).
					valid = false;
				}
			}
			//Error elements(number).
			if (!valid) {
				cout << "Invalid numbers, Please enter again: " << endl;
			}
			else {
				break;
			}
		}
	}
}

void Game::checkMultiInput(string& userCode)
{
	//A function that inputs a custom code, and checks that it doesn't have any repetitions.
	while (true)
	{
		//Multi Player check the code is correct or not.
		checkHiddenCode(userCode);
		//Input the code and show the words.
		inputCode(userCode);
		break;
	}
}

void Game::inputGuess()
{
	//Display which elements you can input and how to get help or quit.

	cout << "Available numbers: [0], [1], [2], [3]\n" << "\t\t   [4], [5], [6], [7]" << endl;
	cout << "You can also Enter [Q] to Quit OR [H] to get HELP OR [A] to Display Game About!" << endl;
	//Create variable to let player input the code.
	string userInputCode;
	//Check the code and input them.
	checkCode(userInputCode);
	//Set the guess code.
	guess.set(userInputCode);
	gTurn++;
}

void Game::inputMultiGuess()
{
	//Display which elements you can input and how to get help or quit.

	cout << "Available numbers: [0], [1], [2], [3]\n" << "\t\t   [4], [5], [6], [7]" << endl;
	cout << "You can also Enter [Q] to Quit OR [H] to get HELP OR [A] to Display Game About!" << endl << "Enter the guessed code: " << endl;
	//Create variable to let player input the code.
	string userInputCode;
	//Check the code and input them.
	checkHiddenCode(userInputCode);
	//Set the guess code.
	guess.set(userInputCode);
	gTurn++;
}

void Game::setCode()
{
	//Set the random code by computer..
	srand((unsigned int)time(NULL));
	//The code is 0 - 7, 8 elements can roll them.
	char hiddenNumbers[9] = { '0', '1', '2', '3', '4', '5', '6', '7' }; //Character array contains the colours Red, Green, Blue, Yellow, Pink, White, Cyan, Magenta
	string randomCode;

	//If round time > 2, then computer will become diffcult.
	if (roundTime > 2) {
		for (int i = 0; i < 4; i++) {
			int randNum = rand() % 8;
			//The elements can be same.
			randomCode += hiddenNumbers[randNum];
		}
	}
	else {
		//If round time < 2, then the computer is not diffcult.
		for (int i = 0; i < 4; i++) {
			int randNum = rand() % 8;
			//Check the same code and put the different into the string.
			if (randomCode.find(hiddenNumbers[randNum]) == string::npos) {
				//Put the element into string.
				randomCode += hiddenNumbers[randNum];
			}
			else {
				//If same, go back and try again.
				i = i - 1;
			}
		}
	}
	//Set the hidden code.
	code.set(randomCode);
}

void Game::inputCode(string userCode)
{
	//Sets the hidden code to the given string s.
	code.set(userCode);
}

void Game::getHint() {
	string getSetCode = code.value();
	srand((unsigned int)time(NULL));
	int randomNum = rand() % 4;
	//countSame variable is used to check hint times and if the time is more than 4, then player cannot get hint.
	//hintTimes variable is used to check how many times the player get hint, and calculate the score.
	//4 same code.
	if ((getSetCode[0] == getSetCode[1]) && (getSetCode[0] == getSetCode[2]) && (getSetCode[0] == getSetCode[3]) && (getSetCode[1] == getSetCode[2]) && (getSetCode[1] == getSetCode[3]) && (getSetCode[2] == getSetCode[3])) {
		if (countSame > 3) {
			cout << "You cannot get more hints in this round!" << endl;
		}
		//Put the hint code into randomHint and show it.
		randomHintCode += getSetCode[randomNum];
		cout << "Give one hint to you, the element is " << getSetCode[randomNum] << endl;
		countSame++;
		hintTimes++;
	}
	//3 same code.
	else if (((getSetCode[0] == getSetCode[1]) && (getSetCode[1] == getSetCode[2])) || ((getSetCode[0] == getSetCode[1]) && (getSetCode[1] == getSetCode[3])) || ((getSetCode[0] == getSetCode[2]) && (getSetCode[2] == getSetCode[3])) || ((getSetCode[1] == getSetCode[2]) && (getSetCode[2] == getSetCode[3]))) {
		int num1 = 0, num2 = 0, num3 = 0;
		//Check the same code location.
		for (int i = 0; i < getSetCode.size(); i++) {
			for (int j = i + 1; j < getSetCode.size(); j++) {
				for (int k = j + 1; k < getSetCode.size(); k++) {
					if (getSetCode[i] == getSetCode[j] == getSetCode[k]) {
						num1 = i;
						num2 = j;
						num3 = k;
					}
				}
			}
		}
		if (countSame > 3) {
			cout << "You cannot get more hints in this round!" << endl;
		}
		else if (countSame == 0) {
			//If randomCode is the same code, and re-roll it.
			while (getSetCode[num1] == getSetCode[randomNum] || getSetCode[num2] == getSetCode[randomNum] || getSetCode[num3] == getSetCode[randomNum]) {
				randomNum = rand() % 4;
				continue;
			}
			//Put the hint code into randomHint and show it.
			randomHintCode += getSetCode[randomNum];
			cout << "Give one hint to you, the element is " << getSetCode[randomNum] << endl;
			countSame++;
			hintTimes++;
			/*for (int i = 0; i < getSetCode.size(); i++) {
				for (int j = i + 1; j < getSetCode.size(); j++) {
					for (int k = j + 1; k < getSetCode.size(); k++) {
						if (getSetCode[i] == getSetCode[j] == getSetCode[k]) {
							num1 = i;
							num2 = j;
							num3 = k;
						}
					}
				}
			}
			while (getSetCode[num1] == getSetCode[randomNum] && getSetCode[num2] == getSetCode[randomNum] && getSetCode[num3] == getSetCode[randomNum]) {
				randomNum = rand() % 4;
				continue;
			}
			randomHintCode += getSetCode[randomNum];
			cout << "Give one hint to you, the element is " << getSetCode[randomNum] << endl;
			countSame++;
			hintTimes++;*/
		}
		else if (countSame == 1) {
			//Put the hint code into randomHint and show it.
			randomHintCode += getSetCode[num1];
			cout << "Give one hint to you, the element is " << getSetCode[num1] << endl;
			countSame++;
			hintTimes++;
			/*while (randomHintCode[0] == getSetCode[randomNum]) {
				randomNum = rand() % 4;
				continue;
			}
			if (!(randomHintCode[0] == getSetCode[randomNum])) {
				randomHintCode += getSetCode[randomNum];
				cout << "Give one hint to you, the element is " << getSetCode[randomNum] << endl;
				countSame++;
				hintTimes++;
			}*/
		}
		else if (countSame == 2) {
			//Put the hint code into randomHint and show it.
			randomHintCode += getSetCode[num2];
			cout << "Give one hint to you, the element is " << getSetCode[num2] << endl;
			countSame++;
			hintTimes++;
		}
		else if (countSame == 3) {
			//Put the hint code into randomHint and show it.
			randomHintCode += getSetCode[num3];
			cout << "Give one hint to you, the element is " << getSetCode[num3] << endl;
			countSame++;
			hintTimes++;
		}
	}
	//2 same code.
	else if (getSetCode[0] == getSetCode[1] || getSetCode[0] == getSetCode[2] || getSetCode[0] == getSetCode[3] || getSetCode[1] == getSetCode[2] || getSetCode[1] == getSetCode[3] || getSetCode[2] == getSetCode[3]) {
		int num1 = 0, num2 = 0;
		//Check the same code location.
		for (int i = 0; i < getSetCode.size(); i++) {
			for (int j = i + 1; j < getSetCode.size(); j++) {
				if (getSetCode[i] == getSetCode[j]) {
					num1 = i;
					num2 = j;
				}
			}
		}
		if (countSame > 3) {
			cout << "You cannot get more hints in this round!" << endl;
		}
		else if (countSame == 0) {
			//If randomCode is the same code, and re-roll it.
			while (getSetCode[num1] == getSetCode[randomNum] || getSetCode[num2] == getSetCode[randomNum]) {
				randomNum = rand() % 4;
				continue;
			}
			randomHintCode += getSetCode[randomNum];
			cout << "Give one hint to you, the element is " << getSetCode[randomNum] << endl;
			countSame++;
			hintTimes++;
		}
		else if (countSame == 1) {
			/*while (getSetCode[num1] == getSetCode[randomNum] || getSetCode[num2] == getSetCode[randomNum]) {
				if (getSetCode[num1] == getSetCode[randomNum] || getSetCode[num2] == getSetCode[randomNum]) {
					randomNum = rand() % 4;
					continue;
				}
				while (randomHintCode[0] == getSetCode[randomNum]) {
					randomNum = rand() % 4;
					continue;
				}
			}*/
			//If randomCode is the same code, and re-roll it.
			while ((getSetCode[num1] == getSetCode[randomNum] || getSetCode[num2] == getSetCode[randomNum]) || (randomHintCode[0] == getSetCode[randomNum])) {
				randomNum = rand() % 4;
				continue;
			}
			if (!(randomHintCode[0] == getSetCode[randomNum])) {
				randomHintCode += getSetCode[randomNum];
				cout << "Give one hint to you, the element is " << getSetCode[randomNum] << endl;
				countSame++;
				hintTimes++;
			}
		}
		/*for (int i = 0; i < getSetCode.size(); i++) {
			for (int j = i + 1; j < getSetCode.size(); j++) {
				if (getSetCode[i] == getSetCode[j]) {
					num1 = i;
					num2 = j;
				}
			}
		}
		while (getSetCode[num1] == getSetCode[randomNum] && getSetCode[num2] == getSetCode[randomNum]) {
			randomNum = rand() % 4;
			continue;
		}
		randomHintCode += getSetCode[randomNum];
		cout << "Give one hint to you, the element is " << getSetCode[randomNum] << endl;
		countSame++;
		hintTimes++;*/
		else if (countSame == 2) {
			/*while (randomHintCode[0] == getSetCode[randomNum] || randomHintCode[1] == getSetCode[randomNum]) {
				randomNum = rand() % 4;
				continue;
			}
			if (!(randomHintCode[0] == getSetCode[randomNum] && randomHintCode[1] == getSetCode[randomNum])) {
				randomHintCode += getSetCode[randomNum];
				cout << "Give one hint to you, the element is " << getSetCode[randomNum] << endl;
				countSame++;
				hintTimes++;
			}*/
			randomHintCode += getSetCode[num1];
			cout << "Give one hint to you, the element is " << getSetCode[num1] << endl;
			countSame++;
			hintTimes++;
		}
		else if (countSame == 3){
			randomHintCode += getSetCode[num2];
			cout << "Give one hint to you, the element is " << getSetCode[num2] << endl;
			countSame++;
			hintTimes++;
		}
	}
	//Not same code.
	else {
		if (countSame > 3) {
			cout << "You cannot get more hints in this round!" << endl;
		}
		else if (countSame == 0) {
			randomHintCode += getSetCode[randomNum];
			cout << "Give one hint to you, the element is " << getSetCode[randomNum] << endl;
			countSame++;
			hintTimes++;
			//system("pause");
			//return inputGuess();
		}
		else if (countSame == 1) {
			//If randomCode is the same code, and re-roll it.
			while (randomHintCode[0] == getSetCode[randomNum]) {
				randomNum = rand() % 4;
				continue;
			}
			if (!(randomHintCode[0] == getSetCode[randomNum])) {
				randomHintCode += getSetCode[randomNum];
				cout << "Give one hint to you, the element is " << getSetCode[randomNum] << endl;
				countSame++;
				hintTimes++;
				//system("pause");
				//return inputGuess();
			}
		}
		else if (countSame == 2) {
			//If randomCode is the same code, and re-roll it.
			while (randomHintCode[0] == getSetCode[randomNum] || randomHintCode[1] == getSetCode[randomNum]) {
				randomNum = rand() % 4;
				continue;
			}
			if (!(randomHintCode[0] == getSetCode[randomNum] && randomHintCode[1] == getSetCode[randomNum])) {
				randomHintCode += getSetCode[randomNum];
				cout << "Give one hint to you, the element is " << getSetCode[randomNum] << endl;
				countSame++;
				hintTimes++;
			}
		}
		else if (countSame == 3) {
			//If randomCode is the same code, and re-roll it.
			while (randomHintCode[0] == getSetCode[randomNum] || randomHintCode[1] == getSetCode[randomNum] || randomHintCode[2] == getSetCode[randomNum]) {
				randomNum = rand() % 4;
				continue;
			}
			if (!(randomHintCode[0] == getSetCode[randomNum] && randomHintCode[1] == getSetCode[randomNum] && randomHintCode[2] == getSetCode[randomNum])) {
				randomHintCode += getSetCode[randomNum];
				cout << "Give one hint to you, the element is " << getSetCode[randomNum] << endl;
				countSame++;
				hintTimes++;
			}
		}
	}
}

string Game::computerBox() {
	//Roll the computer sentence.
	//Computer can speak something to player.
	//It is used the simple if and switch to achieve this function.
	srand((unsigned int)time(NULL));
	int randomChoose = rand() % 4;
	string Box = "";
		if (gTurn > 8) {
		switch (randomChoose) {
		case 0:
			Box = "Hahahahaha you lost! ! ! , I won hahahaha!";
			return Box;
			break;
		case 1:
			Box = "I think your password deciphering skills are really not good. . Haha~";
			return Box;
			break;
		case 2:
			Box = "Don't worry, I won't be proud, it seems you have to work harder too~";
			return Box;
			break;
		case 3:
			Box = "I'm so happy! I defeated the human race!";
			return Box;
			break;
		}
	}
	else if (blackCheck < 2 && whiteCheck >= 2 && whiteCheck <= 4) {
		switch (randomChoose) {
		case 0:
			Box = "It seems that your guess is not wrong, but there is still a long way to go!";
			return Box;
			break;
		case 1:
			Box = "Please don't go on anymore!";
			return Box;
			break;
		case 2:
			Box = "I see how long you can hold on!";
			return Box;
			break;
		case 3:
			Box = "It seems that next time I will be a little harder!";
			return Box;
			break;
		}
	}
	else if (blackCheck == 2 && whiteCheck == 2) {
		switch (randomChoose) {
		case 0:
			Box = "Oh, it looks a bit close. .";
			return Box;
			break;
		case 1:
			Box = "I feel a little misguided. .";
			return Box;
			break;
		case 2:
			Box = "Ouuuu is almost a bit. . . I don't want to be so fast. .";
			return Box;
			break;
		case 3:
			Box = "I really want you to stop! ! !";
			return Box;
			break;
		}
	}
	else if (blackCheck >= 2 && blackCheck < 4 && whiteCheck < 2) {
		switch (randomChoose) {
		case 0:
			Box = "Your correct rate is too high! ! ! I will lose soon! No!";
			return Box;
			break;
		case 1:
			Box = "Who can help me! Although my heart is very cold, but I am so scared!";
			return Box;
			break;
		case 2:
			Box = "You really will succeed soon. . .";
			return Box;
			break;
		case 3:
			Box = "Please be merciful! ! ! !";
			return Box;
			break;
		}
	}
	else if (blackCheck < 2 && whiteCheck < 2) {
		switch (randomChoose) {
		case 0:
			Box = "Looks like your correct rate is not high, haha!";
			return Box;
			break;
		case 1:
			Box = "Don't you want to crack my password!";
			return Box;
			break;
		case 2:
			Box = "Who knows when you can crack the password?";
			return Box;
			break;
		case 3:
			Box = "Hahahaha slowly crack it!";
			return Box;
			break;
		}
	}
	else if (blackCheck == 4 && whiteCheck == 0) {
		switch (randomChoose) {
		case 0:
			Box = "I actually lost! ! ! ! ! ! !";
			return Box;
			break;
		case 1:
			Box = "I actually lost to humans! ! ! !";
			return Box;
			break;
		case 2:
			Box = "I will defeat you one day! ! !";
			return Box;
			break;
		case 3:
			Box = "No! ! ! I will win next time!";
			return Box;
			break;
		}
	}
	return Box;
}

//////////////////////////////////////////////////////////////////////////////////

void Game::displayFile(string filename)
{
	string str;
	ifstream Read(filename);//Find the mastermindAbout.txt
	if (Read.is_open())//If it can open.
	{
		while (!Read.eof())
		{
			getline(Read, str);
			cout << str << endl;//Display the mastermindAbout.txt
		}
		Read.close();
	}
	else//If it cannot open.
	{
		cout << "Unable to open: " << filename << " " << endl;
	}
	cout << endl;
	system("pause");
	//return;
}

int Game::askIntInput(string inputRequire)
{
	int inputNum;
	cout << inputRequire;
	while (!(cin >> inputNum))// If cannot read data from cin stream
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');// Clear current line
		cout << "Please enter again: ";
	}
	return inputNum;
}

string Game::askStrInput(string inputRequire)
{
	string inputString;
	cout << inputRequire;
	cin >> inputString;
	return inputString;
}

char Game::askCharInput(string inputRequire)
{
	char inputChar;
	cout << inputRequire;
	cin >> inputChar;
	return inputChar;
}

void Game::delResult()
{
	char chooseDel;
	chooseDel = askCharInput("Do you want to delete all data? (y/n): ");
	if (chooseDel == 'y')
	{
		fstream Del("savedPlayerData.txt");
		if (!Del.is_open()) //If not exist txt
		{
			cout << " File not found. " << endl;
			cout << endl;
			system("pause");
			return;
		}
		else
		{
			char checkFile;
			Del >> checkFile;
			if (Del.eof())//If the txt is empty.
			{
				cout << " File is empty. " << endl;
				cout << endl;
				Del.close();
				system("pause");
				return;
			}
			else
			{
				fstream Del("savedPlayerData.txt", ios::out);
				Del.close();
				cout << "Data emptied." << endl;
				system("pause");
				return;
			}
		}

	}
	else if (chooseDel == 'n')
	{
		return;
	}
	cout << "Error input!\n";
	system("pause");
	return;
}

void Game::checkResult(Player& playCall)
{
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "######################## Mastermind ###########################\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << "#### The Winner is " << playCall.getName() << "!" << "####\n";
	cout << "#### TotalScore is " << playCall.getCurrentScore() << "!" << "####\n";
	cout << "#### Finished in " << roundTime << "####\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	system("pause");
}

void Game::saveData(Player& playCall)
{
	ofstream Save("savedPlayerData.txt", ios::app);
	Save << "\n";
	Save << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	Save << "######################## Mastermind ###########################\n";
	Save << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	Save << "#### The Winner is " << playCall.getName() << "!" << "####\n";
	Save << "#### TotalScore is " << playCall.getCurrentScore() << "!" << "####\n";
	Save << "#### Finished in " << roundTime << "####\n";
	Save << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	Save.close();
}

void Game::displayResult()
{
	fstream Load("savedPlayerData.txt");
	if (!Load.is_open()) // if not exist txt
	{
		cout << " File not found. " << endl;
		cout << endl;
		system("pause");
		return;
	}
	else // if exist txt
	{
		char checkFile;
		Load >> checkFile;
		if (Load.eof())
		{
			cout << " File is empty. " << endl;
			cout << endl;
			system("pause");
			return;
		}
		else
		{
			string data;
			while (!Load.eof()) // getline untill finish read
			{
				getline(Load, data);
				cout << data << endl;
			}
			Load.close();
			system("pause");
			return;
		}

	}
}

