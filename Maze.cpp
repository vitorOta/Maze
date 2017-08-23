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
int generateRandom(int max){
	if(!randomSeedInitialized){
		srand(time(NULL));
		randomSeedInitialized = true;
	}
	//0-based
	return rand() % max;
}


int randomNextMove(int lastMove, int currentColumn, int currentRow){
	//right or down ++ // left or up --
	int moveTo = -1;
	bool canMove = true; 

	do {
		//4 possible movements
		moveTo = generateRandom(4);

		// after a move, you CAN'T move to his oposite direction (example, go to down, after up)
		bool isOpositeMove = moveTo == lastMove + (lastMove > 1 ? -2 : 2);

		//if column is 0 then can't go to left
		//if column is mazeSize-1 then can't go to right
		//if row is 0 then can't go up
		//if row is mazeSize-1 then can't go down
		bool isPossibleMoveTo = true; 
		switch(moveTo){
			case toRight:
				isPossibleMoveTo = currentColumn < mazeSize - 1;
				break;
			case toDown:
				isPossibleMoveTo = currentRow < mazeSize - 1;
				break;
			case toLeft:
				isPossibleMoveTo = currentColumn > 0;
				break;
			case toUp:
				isPossibleMoveTo = currentRow > 0;
				break;
		}

		canMove = !isOpositeMove && isPossibleMoveTo;
		
		cout << moveTo << " canMove ? " << canMove;

	} while(!canMove);
	return moveTo;
}


void generateMaze(int maze[mazeSize][mazeSize]){
	//TODO see how a int array is preffiled when first initialized
	
	//TODO generate mazes randomly - doing

    int maze[mazeSize][mazeSize];
	int lastMove = -1; // 0 - right // 1 - down // 2 - left // 3 - up

	int markAsGenerated = 99999999; 

	//marking the path
	for (int column = 0; column < mazeSize; column++) {
		for (int row = 0; row < mazeSize; row++) {
			//the position [0][0] is the startPoint
			//the position [mazeSize][mazeSize] is the endPoint
			if((column == 0 && row == 0) || (column == mazeSize-1 && row == mazeSize-1)){
				maze[column][row] = markAsGenerated;
			}else{
				moveTo = randomNextMove(lastMove,column, row);
				maze[column][row] = markAsGenerated;				
				lastMove = moveTo;
			}
		}
	}


	//populating maze
	for (int column = 0; column < mazeSize; column++) {
		for (int row = 0; row < mazeSize; row++) {
			int field = initial;
			if(maze[column][row] != markAsGenerated){
				field = generateRandom(2) == 0 ? wall : initial;				
			}

			maze[column][row] = field;
		}
	}
    return maze;
}

void printMaze(int maze[mazeSize][mazeSize]){
    for (int column = 0;column < 10;column++) {
		for (int row = 0;row < 10;row++) {
			int field = maze[column][row];
			cout << (field == wall ? wallChar : field==player ? playerChar : emptyChar);
		}
		cout << endl;
	}
}


int main(){
	int maze[mazeSize][mazeSize];
	generateMaze(maze);
	printMaze(maze);
	return 0;
}
