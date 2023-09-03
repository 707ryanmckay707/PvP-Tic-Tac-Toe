#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
using namespace std;

enum MENU_OPTIONS
{
	PLAY_GAME_OPT,
	INSTRUCTIONS_OPT,
	QUIT_OPT
};


const int NUM_OF_MENU_OPTIONS = 3;
const int MIN_NUM_OF_ROUNDS = 1;
const int MAX_NUM_OF_ROUNDS = 99;
const int NUM_OF_TIMES_TO_DISPLAY_HELP = 2;


const int ESC_KEY = 27;
const int ARROW_KEY_JUNK = 224;
const int UP_ARROW = 72;
const int LEFT_ARROW = 75;
const int DOWN_ARROW = 80;
const int RIGHT_ARROW = 77;
const int ENTER = 13;
const int SPACEBAR = 32;
const int BACKSPACE = 8;


void gameMasterLoop(const int numOfRounds, int& displayHelpCtr);  //External Function from game.cpp

void displayMainMenu(const int cursorPos);
void updateMenuCursor(int input, int& cursorPos);
int getMenuInput();
void displayInstructions();
int roundSelectorScreen();
void waitForKeyPress(const char keyToPress, const string prompt);
void printTitle();




int main()
{
	bool quit = false;
	int cursorPos = 0;
	int input = 0;

	int displayHelpCtr = NUM_OF_TIMES_TO_DISPLAY_HELP;

	while (quit == false)
	{
		displayMainMenu(cursorPos);
		input = getMenuInput();
		switch (input)
		{
		case ENTER:
		{
			switch (cursorPos)
			{
			case PLAY_GAME_OPT:
			{
				int numOfRounds = roundSelectorScreen();
				if(numOfRounds > 0)
					gameMasterLoop(numOfRounds, displayHelpCtr);
			}
				break;
			case INSTRUCTIONS_OPT:
				displayInstructions();
				break;
			case QUIT_OPT:
				quit = true;
				break;
			}
		}
		case UP_ARROW:
		case DOWN_ARROW:
			updateMenuCursor(input, cursorPos);
			break;
		}
	}

	return 0;
}








void displayMainMenu(const int cursorPos)
{
	system("cls");

	printTitle();

	string mainMenu[NUM_OF_MENU_OPTIONS] = {
	"Play Game\n",
	"Instructions\n",
	"Quit\n"
	};

	for (int count = 0; count < NUM_OF_MENU_OPTIONS; ++count)
	{
		if (count == cursorPos)
			cout << "->";
		else
			cout << "  ";
		cout << mainMenu[count];
	}

	cout << "\n\nUse the Arrow Keys to move the cursor\n";
	cout << "Press Enter to confirm your selection\n";
}








void updateMenuCursor(int input, int& cursorPos)
{
	if ((input == UP_ARROW) && ((cursorPos - 1) >= 0))
		--cursorPos;
	else if ((input == DOWN_ARROW) && ((cursorPos + 1) < NUM_OF_MENU_OPTIONS))
		++cursorPos;
}








int getMenuInput()
{
	int input;
	bool validInput = false;

	do
	{
		input = _getch();
		if (input == ARROW_KEY_JUNK)
			input = _getch();

		switch (input)
		{
		case UP_ARROW:
		case DOWN_ARROW:
		case ENTER:
		case BACKSPACE:
			validInput = true;
			break;
		}
	} while (validInput != true);

	return input;
}








void displayInstructions()
{
	system("cls");

	printTitle();

	cout << "-INSTRUCTIONS-\n\n";

	cout << "X Player's Controls:\n";
	cout << "--------------------\n";
	cout << "Arrow Keys: Move the cursor\n";
	cout << "Enter: Place an X\n\n";

	cout << "O Player's Controls:\n";
	cout << "--------------------\n";
	cout << "WASD (letter keys): Move the cursor\n";
	cout << "Space: Place an O\n\n";

	cout << "Gameplay:\n";
	cout << "---------\n";
	cout << "Players take turns placing their shapes.\n";
	cout << "Whoever gets three in a row, in any direction, wins!\n";
	cout << "Esc: Pauses the Game\n\n";
	
	waitForKeyPress(BACKSPACE, "\nPress Backspace to return to the previous screen.\n");
}








int roundSelectorScreen()
{
	bool stillChoosing = true;
	bool numOfRoundsSelected = true;
	int numOfRounds = 3; // 3 is the number of rounds this screen defaults to

	while (stillChoosing == true)
	{
		system("cls");
		printTitle();
		cout << "            ^\n";
		cout << "Best of: ";
		if (numOfRounds < 10)
			cout << ' ';
		cout << numOfRounds << '\n';
		cout << "            v\n\n\n";

		cout << "Use the Arrow Keys to select the number of rounds\n";
		cout << "Press Enter to confirm your selection\n";
		cout << "Press Backspace to return to the previous screen\n";

		int input = getMenuInput();
		switch (input)
		{
		case UP_ARROW:
		{
			if ((numOfRounds + 2) <= MAX_NUM_OF_ROUNDS)
				numOfRounds += 2;
		}
			break;
		case DOWN_ARROW:
			if ((numOfRounds - 2) >= MIN_NUM_OF_ROUNDS)
			numOfRounds -= 2;
			break;
		case BACKSPACE:
		{
			numOfRounds = -1;
			stillChoosing = false;
		}
			break;
		case ENTER:
			stillChoosing = false;
			break;
		}
	}

	return numOfRounds;
}








void waitForKeyPress(const char keyToPress, const string prompt)
{
	cout << prompt;
	bool keyPressed = false;
	char input;
	do {
		input = _getch();
		if (input == keyToPress)
			keyPressed = true;
	} while (keyPressed == false);
}








void printTitle()
{
	cout << " --------------- \n";
	cout << "|  Tic-Tac-Toe  |\n";
	cout << " --------------- \n\n";
}