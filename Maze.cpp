#include <iostream>
#include <stdlib.h> /* srand, rand */
#include <time.h> /* time */
#include <termios.h>
#include <stdio.h>

using namespace std;

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


/* simulate getche (from https://stackoverflow.com/questions/7469139/what-is-equivalent-to-getch-getche-in-linux)*/


static struct termios old, new1;

/* Initialize new terminal i/o settings */
void initTermios(int echo) 
{
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new1 = old; /* make new settings same as old settings */
  new1.c_lflag &= ~ICANON; /* disable buffered i/o */
  new1.c_lflag &= echo ? ECHO : ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new1); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void) 
{
  tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo) 
{
  char ch;
  initTermios(echo);
  ch = getchar();
  resetTermios();
  return ch;
}

/* Read 1 character without echo */
char getch(void) 
{
  return getch_(0);
}

/* Read 1 character with echo */
char getche(void) 
{
  return getch_(1);
}
/*----------------*/


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
    for (int column = 0; column < mazeSize; column++) {
        for (int row = 0; row < mazeSize; row++) {
            int field = maze[column][row];
            cout << (field == wall ? wallChar : field == player? playerChar : emptyChar);
        }
        cout << endl;
    }
}

bool isPossibleMoveTo(int maze[mazeSize][mazeSize], int moveTo, int currentColumn, int currentRow)
{
    bool isPossibleMoveTo = true;

    //TODO add the check to see if the adjacents positions are walls

    switch (moveTo) {
    case toRight:
        isPossibleMoveTo = currentColumn < mazeSize - 1 && maze[currentColumn + 1][currentRow] != wall;
        break;
    case toDown:
        isPossibleMoveTo = currentRow < mazeSize - 1 && maze[currentColumn][currentRow + 1] != wall;
        break;
    case toLeft:
        isPossibleMoveTo = currentColumn > 0 && maze[currentColumn - 1][currentRow] != wall;
        break;
    case toUp:
        isPossibleMoveTo = currentRow > 0 && maze[currentColumn][currentRow - 1] != wall;
        break;
    }

    return isPossibleMoveTo;
}

int randomNextMove(int maze[mazeSize][mazeSize], int lastMove, int currentColumn, int currentRow)
{
    //right or down ++ // left or up --
    int moveTo = -1;
    bool canMove = true;

    do {
        //4 possible movements
        moveTo = generateRandom(2); //I see a bug here when moving up //TODO fix this bug

        // after a move, you CAN'T move to his oposite direction (example, go to down, after up)
        bool isOpositeMove = (moveTo == lastMove + (lastMove > 1 ? -2 : 2));

        canMove = !isOpositeMove && isPossibleMoveTo(maze, moveTo, currentColumn, currentRow);

    } while (!canMove);

    return moveTo;
}

//TODO see if pass currentColumn and currentRow as a reference is better (if positive, alter his values in the end of the function and remove the param "resultMove[2]")
void moveOnMaze(int maze[mazeSize][mazeSize], int moveTo, int field, int& currentColumn, int& currentRow)
{
    if (!isPossibleMoveTo(maze, moveTo, currentColumn, currentRow)) {
        return;
    }

    switch (moveTo) {
		case toRight:
			currentColumn++;
			break;
		case toDown:
			currentRow++;
			break;
		case toLeft:
			currentColumn-- ;
			break;
		case toUp:
			currentRow--;
			break;
    }
	
    maze[currentColumn][currentRow] = field;
}

void generateMaze(int maze[mazeSize][mazeSize])
{
	//TODO to generate a maze I need see if I'll have escape after
	//(possible solution: see if the next 3 movements before make a move are free)

    int lastMove = -1; // 0 - right // 1 - down // 2 - left // 3 - up

    int markAsGenerated = 99999999;

    int currentColumn = 0;
    int currentRow = 0;

    //initial
    maze[currentColumn][currentRow] = markAsGenerated;
    
    while (!(currentColumn == mazeSize - 1 && currentRow == mazeSize - 1)) {
        //TODO check this, i think that is not correct, but I'm without time to see this now
        int moveTo = randomNextMove(maze, lastMove, currentColumn, currentRow);
        moveOnMaze(maze, moveTo, markAsGenerated, currentColumn, currentRow);
        lastMove = moveTo;
	}

    //populating maze
    for (int column = 0; column < mazeSize; column++) {
        for (int row = 0; row < mazeSize; row++) {
            int field = initial;
            if (maze[column][row] != markAsGenerated) {
                field = generateRandom(3) > 0 ? wall : initial;
            }

            maze[column][row] = field;
        }
    }
}

bool isGameEnd(int maze[mazeSize][mazeSize]){
    return maze[mazeSize][mazeSize] == player;
}

void startGame(int maze[mazeSize][mazeSize]){
    int keyPressed;


    while(!isGameEnd){
        printMaze(maze);
        keyPressed = getche();
    }
    
}


int main()
{
    int maze[mazeSize][mazeSize];
    generateMaze(maze);
    startGame(maze);

    return 0;
}
