/*
 * Commands.c
 *
 *  Created on: Apr 10, 2020
 *      Author: pc
 */

#include <stdio.h>
#include <stdlib.h>
#include "Gurobi.h"
#include <string.h>
#include "linkedListStrategy.h"
#include "game.h"
#include "gameAux.h"

#define N n*m

double *sol;
int *varHash;

/*fill current node at myList with proper details
 * and concatenate it*/
void updateMove(int x, int y) {
	int **list = (int**) malloc(sizeof(int*));
	list[0] = (int*) malloc(sizeof(int) * 3);
	list[0][0] = x;
	list[0][1] = y;
	list[0][2] = gameBoard[x][y].cur;
	update_list(1, &list);
}

/* set functionality */
int set_f(int x, int y, int z) {
	if (currMode == SOLVE && gameBoard[x][y].fixd) {
		printf("this Cell is fixd, change to Edit mode to change it\n");
		return 0;
	} else {
		if (gameBoard[x][y].cur != z) {
			updateMove(x, y);
			gameBoard[x][y].err = check_error(x, y, z);
			gameBoard[x][y].cur = z;
			if (z == 0 && numOfFilledCells > 0) {
				numOfFilledCells--;
				gameBoard[x][y].err = 0;
			} else {
				numOfFilledCells++;
			}
			printf("setting cell (%d , %d) to %d\n",y + 1, x + 1, z);
		} else {
			printf("cell (%d %d) already set to %d\n", y + 1, x + 1, z);
			return 0;
		}
	}
	update_err();
	return 1;
}

int validateBoard() {
	return ILP(0);
}

/* undo functionality */
void undo_f() {
	int i;
	int *move;
	int tmp;

	if (noPrev()) {
		printf("no moves to undo\n");
		return;
	}
	for (i = 0; i < myList->curr->size; i++) {
		move = myList->curr->arr[i];
		printf("%d) changing %d %d from %d to %d\n", i + 1,	move[1] + 1,
				move[0] + 1,gameBoard[move[0]][move[1]].cur, move[2]);
		tmp = gameBoard[move[0]][move[1]].cur;
		gameBoard[move[0]][move[1]].cur = move[2];
		move[2] = tmp;

		if (move[2] != 0 && gameBoard[move[0]][move[1]].cur == 0) {
			numOfFilledCells--;
		}
		if (move[2] == 0 && gameBoard[move[0]][move[1]].cur != 0) {
			numOfFilledCells++;
		}
	}
	getMove(0);
	update_err();
}

/* redo functionality */
void redo_f() {
	int i;
	int tmp;
	int *move;
	if (noNext()) {
		printf("no moves to redo\n");
		return;
	}
	getMove(1);
	for (i = 0; i < myList->curr->size; i++) {
		move = myList->curr->arr[i];
		printf("%d) changing %d %d from %d to %d\n", i + 1,	move[1] + 1,
				 move[0] + 1,gameBoard[move[0]][move[1]].cur, move[2]);

		tmp = gameBoard[move[0]][move[1]].cur;
		gameBoard[move[0]][move[1]].cur = move[2];
		move[2] = tmp;

		if (move[2] != 0 && gameBoard[move[0]][move[1]].cur == 0) {
			numOfFilledCells--;
		}
		if (move[2] == 0 && gameBoard[move[0]][move[1]].cur != 0) {
			numOfFilledCells++;
		}
	}
	update_err();
}

/* hint functionality */
void hint_f(int x, int y) {
	if (ILP(0) != 1) 
		printf("Board is not solvable.\n");
	else
		printf("Aha!, the solution for %d %d is %d\n", x + 1, y + 1, gameBoard[x][y].sol);
}

/*autofill functionality*/
void autoFill_f() {
	int i, j, cur, timesChanged, counter;
	int **moves;
	counter = 0;
	empty_checked();
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			timesChanged = 0;
			if (gameBoard[i][j].cur == 0) {
				for (cur = 1; cur <= N; cur++) {
					if (!check_error(i, j, cur)) {
						timesChanged++;
						gameBoard[i][j].checked = cur;/*indicator*/
						if(timesChanged==2)
							break;

					}
				}
				if (timesChanged != 1) {
					gameBoard[i][j].checked = 0;
				} else {
					counter++;
				}
			}
		}
	}
	if (counter == 0) {
		printf("Nothing to fill\n");
		return;
	}
	/* initilizing moves for undo, redo*/
	moves = (int**) malloc(counter * sizeof(int*));
	if (!moves) {
		printf("failed to allocate memory");
		return;
	}
	for (i = 0; i < counter; i++) {
		moves[i] = (int*) malloc(sizeof(int) * 3);
		if (!moves[i]) {
			printf("failed to allocate memory");
			free_matrix(moves, counter);
			return;
		}
	}

	/* for undo redo functions
	 * creating arr for node that is yet to be created */
	numOfFilledCells += counter;
	counter = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (gameBoard[i][j].checked != 0) {
				moves[counter][0] = i;
				moves[counter][1] = j;
				moves[counter][2] = 0;
				gameBoard[i][j].cur=gameBoard[i][j].checked;
				printf("changing %d %d from 0 to %d\n", j + 1, i + 1,
						gameBoard[i][j].cur);
				gameBoard[i][j].checked = 0;
				counter++;
			}
		}
	}
	update_err();
	update_list(counter, &moves);
}

void guess_f(float threshold) {
	int error, count, counter, sum, i, j, k, indx;
	double randomNum;
	int *vals;
	int **moves;
	double *probs;
	counter = 0;
	srand(3);
	printf("Guessing...\n");
	error = ILP(1);
	if (error != 1) {
		printf("Couldn't guess,Sorry...\n");
		return;
	}
	randomNum = (double) ((double) rand() / (double) RAND_MAX);
	probs = (double*) malloc(sizeof(int) * N);
	vals = (int*) malloc(sizeof(int) * N);
	/* initilizing moves */
	moves = (int*) malloc(N *N sizeof(int*));
	if (!moves) {
		printf("failed to allocate memory");
		free(moves);
		return;
	}
	for (i = 0; i < N*N; i++) {
		moves[i] = (int*) malloc(sizeof(int) * 3);
		if (!moves[i]) {
			printf("failed to allocate memory");
			free_matrix(moves, N*N);
			return;
		}
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (gameBoard[i][j].cur != 0)
				continue;
			count = 0;
			sum = 0;
			for (k = 0; k < N; k++) {
				indx = varHash[calcIndx(i, j, k + 1)];
				/*updating the possible values for cell i,j with there corresponding probabilities*/
				if (indx != -1 && sol[indx] >= threshold && sol[indx] > 0) {
					vals[count] = k + 1;
					probs[count] = sol[indx];
					sum += sol[indx];
					count++;
					if (sum == 1)
						break;
				}

			}
			if (count != 0) {
				for (k = 0; k < N; k++) {
					if (k == 0) {
						probs[k] = (probs[k] / ((double) sum));
					} else {
						probs[k] = ((probs[k] / (double) sum) + probs[k - 1]);

					}
					if (randomNum <= probs[k]&& !check_error(i,j,vals[k])) {
						/*calculates the moves for the redo and undo functions*/
						moves[counter][0] = i;
						moves[counter][1] = j;
						moves[counter][2] = 0;
						gameBoard[i][j].cur=vals[k];
						counter++;
						break;
					}
				}
			}
		}

	}
	update_list(counter, &moves);
	printf("Done guessing\n");
	printBoard();
	free(sol);
	free(varHash);

}
/does the brunt work for the guess_hint function/
void guessHint_f(int i, int j) {
	int error,  k, indx;
	printf("Guessing your Hints...\n");
	error = ILP(1);
	if (error != 1) {
		printf("couldn't Guess your Hint, Sorry....\n");
		return;
	}
	/*probability for value k for the cell i,j is at index varHash[calcIndx]*/
	for (k = 1; k <= N; k++) {
		indx = calcIndx(i, j, k);
		if (varHash[indx] != -1 && sol[varHash[indx]] >0) {
			printf("%d with probability %f\n", k, sol[varHash[indx]]);
		}
	}
	printf("no more values\n");
	free(sol);
	free(varHash);

}
/*exit functionality*/
void exit_f() {
	if (game_already_created == 1)
		free_board();
	printf("Why are you leaving me :(\n Exiting game...\n");
	exit(0);
}

/* for edit default*/
int loadDefault() {
	if (game_already_created == 1)
		free_board();
	n = m = 3;
	if (!malloc_game()) {
		printf("Failed to allocate memory.\n");
		return 0;
	}
	initiate_board();
	return 1;
}

/*reset functionality*/
void reset_board() {
	while (!noPrev())
		undo_f();
}

/* ------------------------------ print function -------------------------*/
/* printBoard functionality */
void printBoard() {
	int row, col;
	for (row = 1; row <= N; row++) {
		if (row == 1)
			printSeparator();
		for (col = 1; col <= N; col++) {
			if (col == 1)
				printf("|");
			if (gameBoard[row - 1][col - 1].cur == 0)
				printf("    ");
			else {
				printf(" %2d", gameBoard[row - 1][col - 1].cur);
				if (gameBoard[row - 1][col - 1].fixd == 1)
					printf(".");
				else if (currMode == EDIT || markerrs == 1) {
					if (gameBoard[row - 1][col - 1].err == 1) {
						printf("*");
					} else
						printf(" ");
				}
			}
			if (col % m == 0)
				printf("|");
			if (col == N)
				printf("\n");
		}
		if (row % n == 0)
			printSeparator();
	}
}
/* --------------------------------------------generate-------------------------------------*/
/* for arr update and restore function in myList*/
void init_arr(int **lst) {
	int i = 0;
	if (!lst) {
		printf("failed to allocate memory.\n");
		return;
	}
	for (; i < N * N; i++) {
		lst[i] = (int*) malloc(3 * sizeof(int));
		if (!lst[i]) {
			free_matrix(lst, N * N);
			printf("failed to allocate memory.\n");
			return;
		}
	}
	return;
}

/** fill arr with indexes and current values*/
void fill_arr(int **lst) {
	int i, j, k = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			lst[k][0] = i;
			lst[k][1] = j;
			lst[k][2] = gameBoard[i][j].cur;
			++k;
		}
	}
	return;
}

/* restoring original board using lst */
void restore(int **lst) {
	int i, j, k;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			gameBoard[i][j].cur = lst[k][2];
			gameBoard[i][j].sol = 0;
			++k;
		}
	}
}

/* generate functionality */
int generate_f(int x, int y) {
	int iteration, status;
	int **lst = (int**) malloc(N * N * sizeof(int*));/* for arr update in myList*/

	if (numOfFilledCells > (N * N - x)) {
		printf("error:not enough empty cells.\n");
		return 0;
	}

	init_arr(lst);
	if (!lst) {
		return 0;
	}
	fill_arr(lst);
	while (iteration < 1000) {
		status = fill_x_random_cells(x, lst);
		if (status == 2) {
			printf("failed to allocate memory");
			return 0;
		}
		if (status) {
			if (ILP(0)) {
				choose_Y_random_cells(y);
				break;
			}
		}
		empty_sol();
		++iteration;
	}
	if (iteration == 1000) {
		free(lst);
		restore(lst);/* restore cur & empty sol*/
		printf("unsuccessful execution.\n");
		return 0;
	}
	update_list(N * N, &lst);
	printBoard();
	printf("finished.\n");
	return 1;
}
/*---------------------------------------- save function -------------------------------*/
/*if index 1,save in edit MODE,0 save in solve MODE*/
int savegameBoard(char *FileName, int index) {
	int i = 0, j = 0;
	FILE *fpointer;
	fpointer = fopen(FileName, "w");
	if (!fpointer) {
		printf("unsuccessful execution for save.\n");
		return 0;
	}
	fprintf(fpointer, "%d %d\n", n, m);
	if (index == 0) {
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				if (gameBoard[i][j].fixd == 0)
					fprintf(fpointer, "%d ", gameBoard[i][j].cur);
				else if (gameBoard[i][j].cur != 0)
					fprintf(fpointer, "%d. ", gameBoard[i][j].cur);
			}
			fprintf(fpointer, "\n");
		}
	} else if (index == 1) {
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				if (gameBoard[i][j].cur != 0) {
					gameBoard[i][j].fixd = 1;
					fprintf(fpointer, "%d. ", gameBoard[i][j].cur);
				} else
					fprintf(fpointer, "%d ", gameBoard[i][j].cur);
			}
			fprintf(fpointer, "\n");
		}
	}
	fclose(fpointer);
	return 1;
}

