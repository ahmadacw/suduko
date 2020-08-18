/*
 * backtracking.c
 *
 *  Created on: Apr 10, 2020
 *      Author: pc
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stack.h"
#include "game.h"
#define NUll 0
#define N n*m

/*part of the init proccess*/
void emptyValid() { /*emptyValidationCells*/
	int i, j;
	for (i = 0; i < n * m; i++) {
		for (j = 0; j < m * n; j++) {
			gameBoard[i][j].vald = 0;
			gameBoard[i][j].checked = 0;
		}
	}
}
/*finds the next empty Cell in the board,which is not in the current solution*/
void findNextEmptyCell(int *i, int *j) {
	int l, k;
	l = *i;
	k = *j;
	int broken = 0;
	for (; l < n * m && !broken; l++) {
		for (k = 0; k < n * m && !broken; k++) {
			if (gameBoard[l][k].vald == 0) {
				broken = 1;
				break;
			}
		}
	}
	l--;
	(*i) = l;
	(*j) = k;
	if (!broken) {
		(*i) = -1;
	}
	return;
}
/*checks the row in which cell row,col resides in, contains val*/

int checkRow(int row, int col, int val) {
	int i;
	for (i = 0; i < n * m; i++) {
		if (gameBoard[row][i].vald == val && col != i) {
			return 0;
		}
	}
	return 1;
}
/*checks the col in which cell row,col resides in, contains val*/

int checkCol(int row, int col, int val) {
	int i;
	for (i = 0; i < n * m; i++) {
		if (gameBoard[i][col].vald == val && row != i) {
			return 0;
		}
	}
	return 1;
}
/*checks the block in which cell row,col resides in, contains val*/
int checkBlock(int row, int col, int val) {
	int x, y, j, l;
	j = row / n;
	l = col / m;
	for (x = j * n; x < j * n + n; x++) {
		for (y = l * m; y < l * m + m; y++) {
			if (gameBoard[x][y].vald == val && x != row && y != col) {
				return 0;
			}
		}
	}
	return 1;
}
/*check if the value works with the current solution*/
int checkVal(int row, int col, int val) {
	return (checkRow(row, col, val) && checkCol(row, col, val)
			&& checkBlock(row, col, val));
}

void initSolver() {
	int i, j;
	for (i = 0; i < n * m; i++) {
		for (j = 0; j < n * m; j++) {
			gameBoard[i][j].vald = gameBoard[i][j].cur;
			gameBoard[i][j].checked = 0;
		}
	}

	return;
}
void resetSolver(Stack *s) {
	emptyValid();
	freeStack(s);
}
/*find the next values which fits the cell i,j*/
int findVal(int i, int j) {
	CELL *c;
	c = &gameBoard[i][j];
	c->checked++;
	while (c->checked < n * m + 1 && !checkVal(i, j, c->checked)) {
		c->checked++;
	}
	if (n * m + 1 == c->checked)
		return 0;
	return 1;
}
int solution() {
	CELL *c;
	Stack *s;
	int i=0, j=0, count=0, noVal=0;

	initSolver();
	s = initStack();
	noVal = 0;
	do {
		findNextEmptyCell(&i, &j);
		if (i == -1 && !noVal) { /* Value found for last node, no empty cells*/
			count++;
			pop(&i, &j, s);
			c = &gameBoard[i][j];
		} else if (noVal) { /* value not found for last node, findNext irrelevant*/
			c->checked = 0;
			c->vald = 0;
			pop(&i, &j, s);
			c = &gameBoard[i][j];
		} else { /* value found, moving on to next thing.*/
			c = &gameBoard[i][j];
			c->checked = 0;
		}
		noVal = 0;
		if (findVal(i, j)) {
			c->vald = c->checked;
			push(i, j, s);
		} else {
			c->vald = 0;
			c->checked = 0;
			noVal = 1;
		}

	} while (!is_Empty(s));
	resetSolver(s);
	return count;
}
