/*
 * assignment1.cpp
 *
 * Word Search Puzzle
 *
 */

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cctype>

using namespace std;

enum direction {
	UP,
	RIGHT_UP,
	LEFT_RIGHT,
	RIGHT_DOWN,
	DOWN,
	LEFT_DOWN,
	RIGHT_LEFT,
	LEFT_UP
};

const int MAX = 50;

struct wordGame
{
	int version;
	int numberRows;
	int numberColumns;
	char puzzle[MAX][MAX]{};
} game;

struct wordFind
{
	string word;
	bool found;
	int row;
	int column;
	direction where;
} theFind;

// function prototypes go here
bool readPuzzle(wordGame &game, string inputFileName)
{
	ifstream input(inputFileName);

	if (input.is_open())
	{
		input >> game.numberRows >> game.numberColumns;

		for (int r = 0; r < game.numberRows; r++)
		{
			for (int c = 0; c < game.numberColumns; c++)
			{
				input >> game.puzzle[r][c];
			}
		}

		game.version = 1;
		input.close();
		return true;
	}

	game.version = 1;
	input.close();
	return false;
}

void displayPuzzle(wordGame &game)
{
	for (int r = 0; r < game.numberRows; r++)
	{
		for (int c = 0; c < game.numberColumns; c++)
		{
			cout << game.puzzle[r][c];
		}

		cout << endl;
	}
}

bool chooseDir(int row, int col, string w)
{
	// Goal of this Method is To Find Whether There is a Match At the Location Given (By Checking All 8 Directions)

	int i, nRow, nCol;

	//Direction :: UP
	nRow = row - 1, nCol = col;

	for (i = 1; i < w.length(); i++)
	{
		//Check Out of Bounds
		if (nRow >= game.numberRows || nRow < 0 || nCol >= game.numberColumns || nCol < 0)
		{
			break;
		}

		//Check If Chars Match
		if (game.puzzle[nRow][nCol] != w[i])
		{
			break;
		}

		//Go "Up" Direction
		nRow -= 1;
	}
	if (i == w.length())
	{
		theFind.where = UP;
		return true;
	}

	//Direction :: RIGHT_UP
	nRow = row - 1, nCol = col + 1;

	for (i = 1; i < w.length(); i++)
	{
		//Check Out of Bounds
		if (nRow >= game.numberRows || nRow < 0 || nCol >= game.numberColumns || nCol < 0)
		{
			break;
		}

		//Check If Chars Match
		if (game.puzzle[nRow][nCol] != w[i])
		{
			break;
		}

		//Go "RIGHT_UP" Direction
		nRow -= 1;
		nCol += 1;
	}
	if (i == w.length())
	{
		theFind.where = RIGHT_UP;
		return true;
	}

	//Direction :: LEFT_RIGHT
	nRow = row, nCol = col + 1;

	for (i = 1; i < w.length(); i++)
	{
		//Check Out of Bounds
		if (nRow >= game.numberRows || nRow < 0 || nCol >= game.numberColumns || nCol < 0)
		{
			break;
		}

		//Check If Chars Match
		if (game.puzzle[nRow][nCol] != w[i])
		{
			break;
		}

		//Go "RIGHT_LEFT" Direction
		nCol += 1;
	}
	if (i == w.length())
	{
		theFind.where = LEFT_RIGHT;
		return true;
	}

	//Direction :: RIGHT_DOWN
	nRow = row + 1, nCol = col + 1;

	for (i = 1; i < w.length(); i++)
	{
		//Check Out of Bounds
		if (nRow >= game.numberRows || nRow < 0 || nCol >= game.numberColumns || nCol < 0)
		{
			break;
		}

		//Check If Chars Match
		if (game.puzzle[nRow][nCol] != w[i])
		{
			break;
		}

		//Go "RIGHT_DOWN" Direction
		nRow += 1;
		nCol += 1;
	}
	if (i == w.length())
	{
		theFind.where = RIGHT_DOWN;
		return true;
	}

	//Direction :: DOWN
	nRow = row + 1, nCol = col;

	for (i = 1; i < w.length(); i++)
	{
		//Check Out of Bounds
		if (nRow >= game.numberRows || nRow < 0 || nCol >= game.numberColumns || nCol < 0)
		{
			break;
		}

		//Check If Chars Match
		if (game.puzzle[nRow][nCol] != w[i])
		{
			break;
		}

		//Go "DOWN" Direction
		nRow += 1;
	}
	if (i == w.length())
	{
		theFind.where = DOWN;
		return true;
	}

	//Direction :: LEFT_DOWN
	nRow = row + 1, nCol = col - 1;

	for (i = 1; i < w.length(); i++)
	{
		//Check Out of Bounds
		if (nRow >= game.numberRows || nRow < 0 || nCol >= game.numberColumns || nCol < 0)
		{
			break;
		}

		//Check If Chars Match
		if (game.puzzle[nRow][nCol] != w[i])
		{
			break;
		}

		//Go "LEFT_DOWN" Direction
		nRow += 1;
		nCol -= 1;
	}
	if (i == w.length())
	{
		theFind.where = LEFT_DOWN;
		return true;
	}

	//Direction :: RIGHT_LEFT
	nRow = row, nCol = col - 1;

	for (i = 1; i < w.length(); i++)
	{
		//Check Out of Bounds
		if (nRow >= game.numberRows || nRow < 0 || nCol >= game.numberColumns || nCol < 0)
		{
			break;
		}

		//Check If Chars Match
		if (game.puzzle[nRow][nCol] != w[i])
		{
			break;
		}

		//Go "RIGHT_LEFT" Direction
		nCol -= 1;
	}
	if (i == w.length())
	{
		theFind.where = RIGHT_LEFT;
		return true;
	}

	//Direction :: LEFT_UP
	nRow = row - 1, nCol = col - 1;

	for (i = 1; i < w.length(); i++)
	{
		//Check Out of Bounds
		if (nRow >= game.numberRows || nRow < 0 || nCol >= game.numberColumns || nCol < 0)
		{
			break;
		}

		//Check If Chars Match
		if (game.puzzle[nRow][nCol] != w[i])
		{
			break;
		}

		//Go "LEFT_UP" Direction
		nRow -= 1;
		nCol -= 1;
	}
	if (i == w.length())
	{
		theFind.where = LEFT_UP;
		return true;
	}

	return false;
}

void findWord(wordGame &game, wordFind &theFind)
{
	theFind.found = false;
	//theFind.where = null;

	for (int r = 0; r < game.numberRows; r++)
	{
		for (int c = 0; c < game.numberColumns + 1; c++)
		{
			string str = theFind.word;
			if ( str[0] == game.puzzle[r][c] )
			{
				if (chooseDir(r, c, str))
				{
					theFind.found = true;
					theFind.row = r + 1;
					theFind.column = c + 1;
					break;
				}
			}
			if (theFind.found == true)
			{
				break;
			}
		}
		if (theFind.found == true)
		{
			break;
		}
	}
}

string enumToString(wordFind &theFind)
{
	switch (theFind.where)
	{
		case UP: return "up";
		case RIGHT_UP: return "right/up";
		case RIGHT_LEFT: return "left";
		case RIGHT_DOWN: return "right/down";
		case DOWN: return "down";
		case LEFT_DOWN: return "left/down";
		case LEFT_RIGHT: return "right";
		case LEFT_UP: return "left/up";
	}
}

int main()
{
	//Input Puzzle
	string filePuzzleName;
	cin >> filePuzzleName;

	//Check and See If Puzzle File is Valid
	if (readPuzzle(game, filePuzzleName))
	{
		cout << "The puzzle from file \"" << filePuzzleName << "\"" << endl;
	}
	else
	{
		cout << "The puzzle file \"" << filePuzzleName << "\" could not be opened or is invalid" << endl;
		//system("pause");
		return 0;
	}

	displayPuzzle(game); //Prints Out Puzzle
		
	//Input Words
	string fileWordsName;
	//cout << "What file would you like to open? :: ";
	cin >> fileWordsName;

	ifstream input(fileWordsName);

	//Check and See If Words File Is Valid
	if (input.is_open())
	{
		string word;

		while (input >> word)
		{
			//int wordLength = static_cast<int>(word.length());

			theFind.word = word;

			findWord(game, theFind);

			if (theFind.found)
			{
				cout << "The word " << theFind.word << " was found at (" << theFind.row << ", " << theFind.column << ") - " << enumToString(theFind) << endl;
			}
			else
			{
				cout << "The word " << theFind.word << " was not found" << endl;
			}
		}
	}
	else
	{
		cout << "The puzzle file \"" << fileWordsName << "\" could not be opened or is invalid" << endl;
		//system("pause");
		return 0;
	}

	// system("pause");

	return 0;
}