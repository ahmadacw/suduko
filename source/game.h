/*
 * game.h
 *
 *  Created on: Apr 10, 2020
 *      Author: pc
 */

#ifndef GAME_H_
#define GAME_H_

int markerrs;/*for mark_errors*/
int n;
int m;
int numOfFilledCells;/*number of filled cells*/
int numOfErrors;
int game_already_created;

typedef struct CELL {
	int cur;/*indicates the value that will be printed to the user*/
	int fixd;/*indicates whether cell is fixed or not in solve mode*/
	int vald;/*field used in backtracking function to try numerous boards*/
	int err;/*indicates whether current value is erroneous*/
	int checked;/*used in backtracking function and generate*/
	int sol;/*for saving solution */
} CELL;

typedef enum MODE {
	INIT, EDIT, SOLVE
} MODE;

enum MODE currMode;/*current mode*/
CELL** gameBoard;

void startGame();
void printBoard();

#endif /* GAME_H_ */
