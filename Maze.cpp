#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <time.h> /* time */
#include<conio.h> /*to Windows getche()*/

using namespace std;

//key codes
const int KEY_D = 100;
const int KEY_S = 115;
const int KEY_A = 97;
const int KEY_W = 119;


//fields
const int initial = 0;
const int wall = 2147483647;
const int player = 2147483646;

//chars
const char playerChar = '@';
const char emptyChar = ' ';
const char wallChar = '#';

//moves
const int toRight = 0;
const int toDown = 1;
const int toLeft = 2;
const int toUp = 3;

//config
const int mazeSize = 20;

bool randomSeedInitialized = false;



int generateRandom(int max)
{
	if (!randomSeedInitialized) {
		srand(time(NULL));
		randomSeedInitialized = true;
	}
	//0-based
	return rand() % max;
}

void printMaze(int maze[mazeSize][mazeSize])
{
	for (int row = 0; row < mazeSize; row++) {
		for (int column = 0; column < mazeSize; column++) {
			int field = maze[row][column];
			cout << (field == wall ? wallChar : field == player ? playerChar : emptyChar);
		}
		cout << endl;
	}
}

bool isPossibleMoveTo(int maze[mazeSize][mazeSize], int moveTo, int currentRow, int currentColumn)
{
	bool isPossibleMoveTo = false;

	//TODO add the check to see if the adjacents positions are walls

	switch (moveTo) {
	case toRight:
		isPossibleMoveTo = currentColumn < mazeSize - 1 && maze[currentRow][currentColumn + 1] != wall;
		break;
	case toDown:
		isPossibleMoveTo = currentRow < mazeSize - 1 && maze[currentRow + 1][currentColumn] != wall;
		break;
	case toLeft:
		isPossibleMoveTo = currentColumn > 0 && maze[currentRow][currentColumn - 1] != wall;
		break;
	case toUp:
		isPossibleMoveTo = currentRow > 0 && maze[currentRow - 1][currentColumn] != wall;
		break;
	}

	return isPossibleMoveTo;
}

int randomNextMove(int maze[mazeSize][mazeSize], int lastMove, int currentRow, int currentColumn)
{
	//right or down ++ // left or up --
	int moveTo = -1;
	bool canMove = true;

	do {
		//4 possible movements
		moveTo = generateRandom(2); //I see a bug here when moving up //TODO fix this bug

									// after a move, you CAN'T move to his oposite direction (example, go to down, after up)
		bool isOpositeMove = (moveTo == lastMove + (lastMove > 1 ? -2 : 2));

		canMove = !isOpositeMove && isPossibleMoveTo(maze, moveTo, currentRow, currentColumn);

	} while (!canMove);

	return moveTo;
}

//TODO rename this method name (I'm not realy taking of a place and putting in another)
bool moveOnMaze(int maze[mazeSize][mazeSize], int moveTo, int field, int& currentRow, int& currentColumn, bool clearCurrent)
{
	if (!isPossibleMoveTo(maze, moveTo, currentRow, currentColumn)) {
		return false;
	}


	bool moved = moveTo == toRight || moveTo == toDown || moveTo == toLeft || moveTo == toUp;


	if (moved) {
		if (clearCurrent) {
			maze[currentRow][currentColumn] = initial;
		}
		switch (moveTo) {
		case toRight:
			currentColumn++;
			break;
		case toDown:
			currentRow++;
			break;
		case toLeft:
			currentColumn--;
			break;
		case toUp:
			currentRow--;
			break;
		}	
		maze[currentRow][currentColumn] = field;
	}
	return moved;
}

void generateMaze(int maze[mazeSize][mazeSize])
{
	//TODO to generate a maze I need see if I'll have escape after
	//(possible solution: see if the next 3 movements before make a move are free)

	int lastMove = -1; // 0 - right // 1 - down // 2 - left // 3 - up

	int markAsGenerated = player;// 99999999;

	int currentRow = 0;
	int currentColumn = 0;

	//initial
	maze[currentRow][currentColumn] = markAsGenerated;

	while (!(currentRow == mazeSize - 1 && currentColumn == mazeSize - 1)) {
		//TODO check this, i think that is not correct, but I'm without time to see this now
		int moveTo = randomNextMove(maze, lastMove, currentRow, currentColumn);
		moveOnMaze(maze, moveTo, markAsGenerated, currentRow, currentColumn,false);
		lastMove = moveTo;
	}

	//populating maze
	for (int row = 0; row < mazeSize; row++) {
		for (int column = 0; column < mazeSize; column++) {
			int field = initial;
			if (maze[row][column] != markAsGenerated) {
				field = generateRandom(3) > 0 ? wall : initial;
			}

			maze[row][column] = field;
		}
	}
}

bool isGameEnd(int maze[mazeSize][mazeSize]) {
	return maze[mazeSize-1][mazeSize - 1] == player;
}

void startGame(int maze[mazeSize][mazeSize]) {
	int currentRow = 0;
	int currentColumn = 0;


	maze[currentRow][currentColumn] = player;

	printMaze(maze);

//	char keyPressed;
	int keyPressed;
	int moveTo=-1;
	int lastMove = -1;

	while (!isGameEnd(maze)) {
		//TODO this getche wasn't work as expected, see this
		//use wdsa for now

		//cout << endl << "-------------" << "Direction (w a s d): ";
		//cin >> keyPressed;

		
		keyPressed = _getche();
		

		switch (keyPressed) {
		case KEY_W://'w':
			moveTo = toUp;
			break;
		case KEY_D://'d':
			moveTo = toRight;
			break;
		case KEY_S://'s':
			moveTo = toDown;
			break;
		case KEY_A://'a':
			moveTo = toLeft;
			break;

		}
		//cout << endl << moveTo << endl;

		bool moved = moveOnMaze(maze, moveTo, player, currentRow, currentColumn, true);
		
		lastMove = moveTo;
		system("cls");
		printMaze(maze);
	}


	cout << endl << " ----- E N D ----- " << endl;
}


int main()
{
	int maze[mazeSize][mazeSize];
	generateMaze(maze);
	startGame(maze);
	system("pause");
	return 0;
}
