#include<iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

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
const int mazeSize = 10;

bool randomSeedInitialized = false;
int generateRandom(int max) {
    if (!randomSeedInitialized) {
        srand(time(NULL));
        randomSeedInitialized = true;
    }
    //0-based
    return rand() % max;
}

bool isPossibleMoveTo(int maze[mazeSize][mazeSize], int moveTo, int currentColumn, int currentRow){
    bool isPossibleMoveTo = true;

//TODO add the check to see if the adjacents positions are wall's

        switch (moveTo) {
		case toRight:
		    isPossibleMoveTo = currentColumn < mazeSize - 1 || maze[currentColumn + 1][currentRow] != wall ;
		    break;
		case toDown:
		    isPossibleMoveTo = currentRow < mazeSize - 1  || maze[currentColumn][currentRow + 1] != wall;
		    break;
		case toLeft:
		    isPossibleMoveTo = currentColumn > 0  || maze[currentColumn - 1][currentRow] != wall;
		    break;
		case toUp:
		    isPossibleMoveTo = currentRow > 0 || maze[currentColumn][currentRow - 1] != wall;
		    break;
	}

    return isPossibleMoveTo;
}

int randomNextMove(int lastMove, int currentColumn, int currentRow) {
    //right or down ++ // left or up --
    int moveTo = -1;
    bool canMove = true;

    do {
        //4 possible movements
        moveTo = generateRandom(4);

        // after a move, you CAN'T move to his oposite direction (example, go to down, after up)
        bool isOpositeMove = (moveTo == lastMove + (lastMove > 1 ? -2 : 2));

        canMove = !isOpositeMove && isPossibleMoveTo(maze,moveTo, currentColumn,currentRow);

    } while (!canMove);

    return moveTo;
}

//TODO see if pass currentColumn and currentRow as a reference is better (if positive, alter his values in the end of the function and remove the param "resultMove[2]")
void moveOnMaze(int maze[mazeSize][mazeSize], int moveTo, int field, int& currentColumn, int& currentRow){
	//TODO implement
//resultMove is an array that returns the column([0]) and the row([1])
//field is the char to print on the desired move; 

	if(!isPossibleMoveTo(maze, moveTo, currentColumn, currentRow)){
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
		    currentColumn-;
		    break;
		case toUp:
		    currentRow--;
		    break;
	}
	maze[currentColumn][currentRow] = field;
	
}

void generateMaze(int maze[mazeSize][mazeSize]) {
    //TODO see how a int array is preffiled when first initialized

    int maze[mazeSize][mazeSize];
    int lastMove = -1; // 0 - right // 1 - down // 2 - left // 3 - up

    int markAsGenerated = 99999999;

    int currentColumn = 0;
    int currentRow = 0;

    while (currentColumn != mazeSize - 1 && currentRow != mazeSize - 1) {
//TODO check this, i think that is not correct, but I'm without time to see this now
        moveOnMaze(maze, moveTo, markAsGenerated, currentColumn, currentRow);

        moveTo = randomNextMove(lastMove, currentColumn, currentRow);       
	lastMove = moveTo;

    }

    //populating maze
    for (int column = 0; column < mazeSize; column++) {
        for (int row = 0; row < mazeSize; row++) {
            int field = initial;
            if (maze[column][row] != markAsGenerated) {
                field = generateRandom(2) == 0 ? wall : initial;
            }

            maze[column][row] = field;
        }
    }
    return maze;
}

void printMaze(int maze[mazeSize][mazeSize]) {
    for (int column = 0; column < 10; column++) {
        for (int row = 0; row < 10; row++) {
            int field = maze[column][row];
            cout << (field == wall ? wallChar : field == player ? playerChar : emptyChar);
        }
        cout << endl;
    }
}

int main() {
    int maze[mazeSize][mazeSize];
    generateMaze(maze);
    printMaze(maze);
    return 0;
}
