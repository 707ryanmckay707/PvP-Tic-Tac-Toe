#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
using namespace std;

enum GAME_STATE
{
	QUIT_STATE,
	PAUSED_STATE,
	GAMEPLAY_STATE,
	X_ROUND_WIN_STATE,
	O_ROUND_WIN_STATE,
	TIED_ROUND_STATE,
	X_GAME_WIN_STATE,
	O_GAME_WIN_STATE,
	TIED_GAME_STATE
};

enum WIN_TYPE
{
	ROW0_WIN,
	ROW1_WIN,
	ROW2_WIN,
	COL0_WIN,
	COL1_WIN,
	COL2_WIN,
	DIAGONAL0_WIN,
	DIAGONAL2_WIN
};

enum MENU_OPTIONS
{
	PLAY_GAME_OPT,
	INSTRUCTIONS_OPT,
	QUIT_OPT
};


const int ORDERED_PAIR = 2;
const int iX_COORD = 0;					//index constant
const int iY_COORD = 1;					//index constant
const int iCURR_ROUND_NUM = 0;			//index constant
const int iNUM_OF_ROUNDS = 1;			//index constant

const int NUM_OF_MENU_OPTIONS = 3;
const int BOARD_DIM = 3;
const int MIN_NUM_OF_ROUNDS = 1;
const int MAX_NUM_OF_ROUNDS = 99;
const int MAX_TURN_COUNT = 9;

const int NUM_OF_PLAYERS = 2;
const int iX_SCORE = 0;					//index constant
const int iO_SCORE = 1;					//index constant

const int ESC_KEY = 27;
const int ARROW_KEY_JUNK = 224;
const int UP_ARROW = 72;
const int LEFT_ARROW = 75;
const int DOWN_ARROW = 80;
const int RIGHT_ARROW = 77;
const int ENTER = 13;
const int SPACEBAR = 32;
const int BACKSPACE = 8;


const int WHITE_TEXT_BLACK_BACKGROUND = (15 + 0*16);
const int WHITE_TEXT_DARK_GREEN_BACKGROUND = (15 + 2*16);
const int WHITE_TEXT_CORAL_BACKGROUND = (15 + 3*16);

const int LEN_OF_SCOREBOARD = 21;


void displayMainMenu(const int cursorPos);
void updateMenuCursor(int input, int& cursorPos);
int getMenuInput();
void displayInstructions();
int roundSelectorScreen();
void waitForKeyPress(const char key, const string prompt);
void printTitle();

void gameMasterLoop(const int numOfRounds);
char genRandomTurn();
bool gameRoundLoop(const int roundInfo[ORDERED_PAIR], int score[ORDERED_PAIR], char whoseTurn);
int pauseScreen();
void printRoundOverFooter(const int gameState, const bool continuePlaying);
void printScoreBoard(const char whoseTurn, const int gameState, const int roundInfo[ORDERED_PAIR], const int score[ORDERED_PAIR]);
void printGameplayBoard(const char board[BOARD_DIM][BOARD_DIM], const char whoseTurn, const int cursor[ORDERED_PAIR], const int turnNum, HANDLE hConsole);
void printRoundOverBoard(const char board[BOARD_DIM][BOARD_DIM], const int gameState, const int winType, HANDLE hConsole);
int getGameInput(const char whoseTurn);
bool placeShape(char board[BOARD_DIM][BOARD_DIM], const int cursor[ORDERED_PAIR], const char whoseTurn);
bool checkForRoundWin(const char board[BOARD_DIM][BOARD_DIM], const char playerShape, int& winType);
void updateGameCursor(int cursor[ORDERED_PAIR], const int input);

int main()
{
	bool quit = false;
	int cursorPos = 0;
	int input = 0;

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
					gameMasterLoop(numOfRounds);
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
	cout << "Enter: Place an O\n\n";

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
	int numOfRounds = 3;

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
			if((numOfRounds + 2) <= MAX_NUM_OF_ROUNDS)
				numOfRounds += 2;
		}
			break;
		case DOWN_ARROW:
			if((numOfRounds - 2) >= MIN_NUM_OF_ROUNDS)
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

void waitForKeyPress(const char key, const string prompt)
{
	cout << prompt;
	bool enterPressed = false;
	char input;
	do {
		input = _getch();
		if (input == key)
			enterPressed = true;
	} while (enterPressed == false);
}

void printTitle()
{
	cout << " --------------- \n";
	cout << "|  Tic-Tac-Toe  |\n";
	cout << " --------------- \n\n";
}







void gameMasterLoop(const int numOfRounds)
{
	int roundInfo[ORDERED_PAIR];
	roundInfo[iCURR_ROUND_NUM] = 0;
	roundInfo[iNUM_OF_ROUNDS] = numOfRounds;

	int score[NUM_OF_PLAYERS] = { 0, 0 };
	bool continuePlaying = true;

	while (continuePlaying == true)
	{
		++roundInfo[iCURR_ROUND_NUM];
		char whoGoesFirst = genRandomTurn();
		continuePlaying = gameRoundLoop(roundInfo, score, whoGoesFirst);
	}
}

char genRandomTurn()
{
	srand(static_cast<int>(time(0)));
	int randNum = (rand() % 2);
	if (randNum == 0)
		return 'X';
	else
		return 'O';
}

bool gameRoundLoop(const int roundInfo[ORDERED_PAIR], int score[ORDERED_PAIR], char whoseTurn)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	char board[BOARD_DIM][BOARD_DIM] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
	int gameState = GAMEPLAY_STATE;
	int winType;

	int cursor[ORDERED_PAIR] = {0,0};

	int input = 0;
	int turnNum = 1;

	while (gameState == GAMEPLAY_STATE)		//the game runs until an end state is reached, a win, tie or quit state
	{
		system("cls");
		printScoreBoard(whoseTurn, gameState, roundInfo, score);
		printGameplayBoard(board, whoseTurn, cursor, turnNum, hConsole);

		input = getGameInput(whoseTurn);
		switch (input)
		{
		case ENTER:
		case SPACEBAR:
			{
				if (placeShape(board, cursor, whoseTurn))			//if a shape was placed, evaluate the board
				{
					if (checkForRoundWin(board, whoseTurn, winType))		//if someone won, determine who won, and where
					{
						if (whoseTurn == 'X')
							gameState = X_ROUND_WIN_STATE;
						else
							gameState = O_ROUND_WIN_STATE;
					}
					else									//if no-one won, prepare for next turn
					{
						if (whoseTurn == 'X')				//Change whose turn it is
							whoseTurn = 'O';
						else
							whoseTurn = 'X';
						++turnNum;							//Increase turn count and see if maxNum of turns have been played
						if (turnNum > MAX_TURN_COUNT)
							gameState = TIED_ROUND_STATE;
					}

				}
			}
			break;
		case ESC_KEY:
			{
				gameState = PAUSED_STATE;
				while (gameState == PAUSED_STATE)
				{
					system("cls");
					printScoreBoard(whoseTurn, gameState, roundInfo, score);
					int invisibleCursor[ORDERED_PAIR] = { -1,-1 };
					printGameplayBoard(board, whoseTurn, invisibleCursor, turnNum, hConsole);

					gameState = pauseScreen();
				}
			}
			break;
		case 'I':
		case 'i':
			displayInstructions();
			break;
		default:
			updateGameCursor(cursor, input);
		}
	}

	boolean continuePlaying;
	if (gameState == QUIT_STATE)
	{
		continuePlaying = false;
	}
	else
	{
		if (gameState == X_ROUND_WIN_STATE)
			++score[iX_SCORE];
		else if (gameState == O_ROUND_WIN_STATE)
			++score[iO_SCORE];

		if (score[iX_SCORE] >= (roundInfo[iNUM_OF_ROUNDS]/2 + 1))
		{
			continuePlaying = false;
			gameState = X_GAME_WIN_STATE;
		}
		else if (score[iO_SCORE] >= (roundInfo[iNUM_OF_ROUNDS]/2 + 1))
		{
			continuePlaying = false;
			gameState = O_GAME_WIN_STATE;
		}
		else if (roundInfo[iCURR_ROUND_NUM] == roundInfo[iNUM_OF_ROUNDS])
		{
			continuePlaying = false;
			if (score[iX_SCORE] > score[iO_SCORE])
				gameState = X_GAME_WIN_STATE;
			else if (score[iO_SCORE] > score[iX_SCORE])
				gameState = O_GAME_WIN_STATE;
			else
				gameState = TIED_GAME_STATE;
		}
		else
		{
			continuePlaying = true;
		}

		system("cls");
		printScoreBoard(whoseTurn, gameState, roundInfo, score);
		printRoundOverBoard(board, gameState, winType, hConsole);
		printRoundOverFooter(gameState, continuePlaying);
		
	}
	return continuePlaying;
}







int pauseScreen()
{
	cout << "\nPress Esc to resume\n";
	cout << "Press I for the instructions\n\n";
	cout << "Press M to return to the main menu\n";
	cout << "(Progress will not be saved)\n\n";

	char input;
	bool validInput = false;
	int gameState;
	do
	{
		input = _getch();

		switch (input)
		{
		case 'm':
		case 'M':
			gameState = QUIT_STATE;
			validInput = true;
			break;
		case ESC_KEY:
			gameState = GAMEPLAY_STATE;
			validInput = true;
			break;
		case 'I':
		case 'i':
			displayInstructions();
			gameState = PAUSED_STATE;
			validInput = true;
			break;
		}
	} while (validInput == false);
	
	return gameState;
}







void printRoundOverFooter(const int gameState, const bool continuePlaying)
{
	switch (gameState)
	{
	case X_ROUND_WIN_STATE:
		cout << "\nX wins the round!";
		break;
	case O_ROUND_WIN_STATE:
		cout << "\nO wins the round!";
		break;
	case TIED_ROUND_STATE:
		cout << "\nThis rounds a tie.";
		break;
	case X_GAME_WIN_STATE:
		cout << "\nX wins the game!";
		break;
	case O_GAME_WIN_STATE:
		cout << "\nO wins the game!";
		break;
	case TIED_GAME_STATE:
		cout << "\nThe game ended in a tie. This is very unexpected.";
		break;
	}

	if (continuePlaying == true)
		waitForKeyPress(ENTER, "\nPress enter to proceed to the next round.\n");
	else
		waitForKeyPress(ENTER, "\nPress enter to the main menu.\n");
}







void printScoreBoard(const char whoseTurn, const int gameState, const int roundInfo[ORDERED_PAIR], const int score[ORDERED_PAIR])
{
	string message;

	switch (gameState)
	{
	case PAUSED_STATE:
		message = "Paused";
		break;
	case GAMEPLAY_STATE:
		message = whoseTurn;
		message += "'s Turn";
		break;
	case X_ROUND_WIN_STATE:
	case O_ROUND_WIN_STATE:
	case TIED_ROUND_STATE:
		message = "Round Over";
		break;
	case X_GAME_WIN_STATE:
	case O_GAME_WIN_STATE:
	case TIED_GAME_STATE:
		message = "Game Over";
		break;
	}
	
	string horizLine(LEN_OF_SCOREBOARD, '-');
	cout << ' '<< horizLine << '\n';
	
	cout << '|';
	cout << setw(10) << right << "Score: ";

	cout << setw(11) << left << (to_string(score[iX_SCORE]) + "-" + to_string(score[iO_SCORE]));
	cout << "|\n";
	
	cout << '|'; 
	cout << setw(10) << right << "Round: ";
	cout << setw(11) << left << (to_string(roundInfo[iCURR_ROUND_NUM]) + "/" + to_string(roundInfo[iNUM_OF_ROUNDS]));
	cout << "|\n";

	cout << '|';
	cout << setw(10) << right << "Message: ";
	cout << setw(11) << left << message;
	cout << "|\n";

	cout << ' ' << horizLine << "\n\n";
}







void printGameplayBoard(const char board[BOARD_DIM][BOARD_DIM], const char whoseTurn, const int cursor[ORDERED_PAIR], const int turnNum, HANDLE hConsole)
{
	for (int row = 0; row < BOARD_DIM; ++row)
	{
		//cout << "        ";
		for (int col = 0; col < BOARD_DIM; ++col)
		{
			if (row == cursor[iY_COORD] && col == cursor[iX_COORD])
			{
				SetConsoleTextAttribute(hConsole, WHITE_TEXT_DARK_GREEN_BACKGROUND);
				cout << board[row][col];
				SetConsoleTextAttribute(hConsole, WHITE_TEXT_BLACK_BACKGROUND);
			}
			else
			{
				cout << board[row][col];
			}

			cout << ' ';

			if (col < (BOARD_DIM - 1))
				cout << '*';

			cout << ' ';
		}

		cout << '\n';
		
		if (row < (BOARD_DIM - 1))
		{
			//cout << "        ";
			for (int count = 0; count < (BOARD_DIM + 6); ++count)
				cout << '*';
			cout << '\n';
		}
	}
}







void printRoundOverBoard(const char board[BOARD_DIM][BOARD_DIM], const int gameState, const int winType, HANDLE hConsole)
{
	for (int row = 0, revCount = (BOARD_DIM - 1); row < BOARD_DIM; ++row, --revCount)
	{
		for (int col = 0; col < BOARD_DIM; ++col)
		{
			if (
					(winType == row) 
				||	(winType == (col + COL0_WIN)) 
				||	((winType == DIAGONAL0_WIN) && (row == col))
				||	((winType == DIAGONAL2_WIN) && (revCount == col))
				)
			{
				SetConsoleTextAttribute(hConsole, WHITE_TEXT_CORAL_BACKGROUND);
				cout << board[row][col];
				SetConsoleTextAttribute(hConsole, WHITE_TEXT_BLACK_BACKGROUND);
			}
			else
			{
				cout << board[row][col];
			}

			cout << ' ';

			if (col < (BOARD_DIM - 1))
				cout << '*';

			cout << ' ';
		}

		cout << '\n';

		if (row < (BOARD_DIM - 1))
		{
			for (int count = 0; count < (BOARD_DIM + 6); ++count)
				cout << '*';
			cout << '\n';
		}
	}
}







int getGameInput(const char whoseTurn)
{
	int input;
	bool validInput = false;
	bool helpMSGDisplayed = false;

	do
	{
		input = _getch();
		if (input == ARROW_KEY_JUNK)
			input = _getch();

		switch (input)
		{
		case UP_ARROW:
		case LEFT_ARROW:
		case DOWN_ARROW:
		case RIGHT_ARROW:
		case ENTER:
		{
			if (whoseTurn == 'X')
				validInput = true;
			break;
		}
		case 'w':
		case 'W':
		case 'a':
		case 'A':
		case 's':
		case 'S':
		case 'd':
		case 'D':
		case SPACEBAR:
		{
			if (whoseTurn == 'O')
				validInput = true;
			break;
		}
		case ESC_KEY:
		case 'I':
		case 'i':
			validInput = true;
			break;
		}

		if (validInput == false && helpMSGDisplayed == false)
		{
			cout << "\nPress Esc to pause the game and get a list of options.\n";
			helpMSGDisplayed = true;
		}
	} while (validInput != true);

	return input;
}







bool placeShape(char board[BOARD_DIM][BOARD_DIM], const int cursor[ORDERED_PAIR], const char whoseTurn)
{
	bool ableToPlaceShape = false;

	if (board[cursor[iY_COORD]][cursor[iX_COORD]] == ' ')
	{
		board[cursor[iY_COORD]][cursor[iX_COORD]] = whoseTurn;
		ableToPlaceShape = true;
	}
	
	return ableToPlaceShape;
}







bool checkForRoundWin(const char board[BOARD_DIM][BOARD_DIM], const char whoseTurn, int& winType)
{
	int numberInALine;
	bool playerWon = false;

	//Check for a row based win
	for (int row = 0; ((row < BOARD_DIM) && (playerWon != true)); ++row)
	{
		numberInALine = 0;
		for (int col = 0; col < BOARD_DIM; ++col)
		{
			if (board[row][col] == whoseTurn)
				++numberInALine;
		}
		if (numberInALine == 3)
		{
			playerWon = true;
			winType = row;		//Set win type to the corresponding row
		}
			
	}

	//Check for a column based win
	for (int col = 0; ((col < BOARD_DIM) && (playerWon != true)); ++col)
	{
		numberInALine = 0;
		for (int row = 0; row < BOARD_DIM; ++row)
		{
			if (board[row][col] == whoseTurn)
				++numberInALine;
		}
		if (numberInALine == 3)
		{
			playerWon = true;
			winType = COL0_WIN + col;	//Set win type to the corresponding col, using COL0_WIN as an offset
		}
			
	}

	//check for diagonal win from upper left to bottom right
	numberInALine = 0;
	for (int count = 0; count < BOARD_DIM; count++)
	{
		if (board[count][count] == whoseTurn)
			++numberInALine;
	}
	if (numberInALine == 3)
	{
		playerWon = true;
		winType = DIAGONAL0_WIN;
	}
		

	//check for diagonal win from bottom left to upper right
	numberInALine = 0;
	for (int row = 2, col = 0; col < BOARD_DIM; --row, ++col)
	{
		if (board[row][col] == whoseTurn)
			++numberInALine;
	}
	if (numberInALine == 3)
	{
		playerWon = true;
		winType = DIAGONAL2_WIN;
	}

	return playerWon;
}







void updateGameCursor(int cursor[ORDERED_PAIR], const int input)
{
	switch (input)
	{
	case UP_ARROW:
	case 'w':
	case 'W':
		if ((cursor[iY_COORD] - 1) >= 0)
			--cursor[iY_COORD];
		break;
	case LEFT_ARROW:
	case 'a':
	case 'A':
		if ((cursor[iX_COORD] - 1) >= 0)
			--cursor[iX_COORD];
		break;
	case DOWN_ARROW:
	case 's':
	case 'S':
		if ((cursor[iY_COORD] + 1) < BOARD_DIM)
			++cursor[iY_COORD];
		break;
	case RIGHT_ARROW:
	case 'd':
	case 'D':
		if ((cursor[iX_COORD] + 1) < BOARD_DIM)
			++cursor[iX_COORD];
		break;
	}
}