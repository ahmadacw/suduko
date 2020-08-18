/*
 * game.c
 *
 *  Created on: Apr 10, 2020
 *      Author: pc
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backtracking.h"
#include "parser.h"
#include "Commands.h"
#include "game.h"
#include "gameAux.h"

#define N n*m

void solve_command() {
	MODE prev = currMode;
	currMode = SOLVE;
	if (loadgameBoard(inp->second)) {
		printBoard();
		if (numOfFilledCells == N * N) {
			if (erroneous(gameBoard))
				printf("current solution is erroneous.\n");
			else {
				currMode = INIT;
				printf("Board Solved successfully.\n");
			}
		}
	} else {
		currMode = prev;
		if (game_already_created) {
			printf("printing previous Board.\n");
			printBoard();
		}
	}
}

void edit_command() {
	if (inp->second[0] == '\0') {
		if (!loadDefault())
			return;
		currMode = EDIT;
		printBoard();
	} else if (loadgameBoard(inp->second)) {
		currMode = EDIT;
		printBoard();
	}
}

void mark_errors_command() {
	if (currMode != SOLVE)
		printf("This command is only available in Solve mode.\n");
	else {
		markerrs = atoi(inp->second);
		printBoard();
	}
}

void print_board_command() {
	if (currMode == INIT) {
		printf("This command is only available in Edit and Solve modes.\n");
	} else
		printBoard();
}

void set_command() {
	if (currMode == INIT) {
		printf("This command is only available in Edit and Solve modes\n");
	/* at our code we by mistake dealed with the first input as
	 * it indicates the rows instead of coloumns so you might see
	 * at someplaces that we switched them like here
	 *  to make things right, sorry :(
	 */
	} else if (set_f( atoi(inp->third) - 1,atoi(inp->second) - 1,
			atoi(inp->fourth))) {
		if (currMode == SOLVE) {
			if (numOfFilledCells == N * N) {
				if (validateBoard()) {
					currMode=INIT;
					printf("Board Solved successfully.\n");
				} else {
					printf("Board is erroneous.\n");
					return;
				}
			}
		}
		printBoard();
	}
}

void validate_command() {
	if (currMode == INIT)
		printf("This command is only available in Edit and Solve modes.\n");
	else {
		if (erroneous(gameBoard))
			printf("Error: Board is erroneous.\n");
		else if (validateBoard())
			printf("Board is solvable.\n");
		else
			printf("Board is unsolvable.\n");
	}
}

void guess_command() {
	if (currMode != SOLVE)
		printf("Command only available in solve mode.\n");
	else {
		if (erroneous(gameBoard))
			printf("Error, Board is erroneous.\n");

		else
			guess_f(atoi(inp->second) - 1);
	}
}

void generate_command() {
	if (currMode == EDIT) {
		generate_f(atoi(inp->second), atoi(inp->third));
	} else
		printf("Command only available in edit mode.\n");
}

void undo_command() {
	if (currMode == INIT)
		printf("This command is only available in Edit and Solve modes.\n");
	else {
		undo_f();
		printBoard();
	}
}

void redo_command() {
	if (currMode == INIT)
		printf("This command is only available in Edit and Solve modes.\n");
	else {
		redo_f();
		printBoard();
	}
}

void save_command() {
	if (currMode == INIT) {
		printf("This command is only available in Edit and Solve modes.\n");
		return;
	} else if (currMode == EDIT) {
		if (erroneous(gameBoard)) {
			printf("Error, Couldnt save, Board is erroneous.\n");
			return;
		}
		if (!validateBoard()) {
			printf("Error, Couldnt save, Board is not solvable.\n");
			return;
		}
		if (savegameBoard(inp->second, 1))
			printf("Saved to : %s\n", inp->second);
	} else {
		if (savegameBoard(inp->second, 0))
			printf("Saved to : %s\n", inp->second);
	}
}

void hint_command() {

	if (currMode != SOLVE)
		printf("Command only available in solve mode.\n");
	else {
		if (erroneous(gameBoard))
			printf("Error, Board is erroneous.\n");

		else if (gameBoard[atoi(inp->third) - 1][atoi(inp->second) - 1].fixd
				== 1)
			printf("Error, Cell is fixed.\n");

		else if (gameBoard[atoi(inp->third) - 1][atoi(inp->second) - 1].cur
				!= 0)
			printf("Error, Cell is already has a value");
		else
			hint_f(atoi(inp->third) - 1, atoi(inp->second) - 1);
	}
}

void guess_hint_command() {
	if (currMode != SOLVE)
		printf("Command only available in solve mode.\n");
	else {
		if (erroneous(gameBoard))
			printf("Error, Board is erroneous.\n");

		else if (gameBoard[atoi(inp->third) - 1][atoi(inp->second) - 1].fixd
				== 1)
			printf("Error, Cell is fixed.\n");

		else if (gameBoard[atoi(inp->third) - 1][atoi(inp->second) - 1].cur
				!= 0)
			printf("Error, Cell is already has a value");
		else
			guessHint_f( atoi(inp->third) - 1,atoi(inp->second) - 1);
	}
}

void num_solutions_command() {
	if (currMode == INIT)
		printf("This command is only available in Edit and Solve modes.\n");
	else {
		if (erroneous(gameBoard))
			printf("Error, Board is erroneous.\n");
		else {
			printf("number of solutions for the current board is %d\n",
					solution());

		}
	}
}

void autofill_command() {
	if (currMode == INIT)
		printf("This command is only available in Solve mode.\n");
	else if (erroneous(gameBoard))
		printf("Error, Board is erroneous.\n");
	else {
		autoFill_f();
		printBoard();
		if (numOfFilledCells == N * N)
			printf("Board Solved successfully.\n");
	}
}

void reset_command() {
	if (currMode == INIT)
		printf("This command is only available in Edit and Solve modes.\n");
	else {
		reset_board();
		printBoard();
		printf("Board have been reset\n");
	}
}

void exit_command() {
	if (game_already_created)
		free_board();
	game_already_created = 0;
	printf("Exiting game...\n");
}

void startGame() {
	markerrs = 1;
	currMode = INIT;
	printf("Welcome to sudoku console\n");
	while (1) {
		printf("Enter a command from the list attached to the game\n");
		recieve_Input();
		check_Input();

		switch (inp->command) {
		case invalid:
			printf("Invalid command.\n");
			break;
		case solve: /*done*/
			solve_command();
			break;
		case edit: /*done*/
			edit_command();
			break;
		case mark_errors: /*done*/
			mark_errors_command();
			break;
		case print_board: /*done*/
			print_board_command();
			break;
		case set: /*done*/
			set_command();
			break;
		case validate: /*LP*/
			validate_command();
			break;
		case guess: /*LP*/
			guess_command();
			break;
		case generate: /*LP*/
			generate_command();
			break;
		case undo: /*done*/
			undo_command();
			break;
		case redo: /*done*/
			redo_command();
			break;
		case save: /*done*/
			save_command();
			break;
		case hint: /*LP*/
			hint_command();
			break;
		case guess_hint: /*LP*/
			guess_hint_command();
			break;
		case num_solutions: /*done*/
			num_solutions_command();
			break;
		case autofill: /*done*/
			autofill_command();
			break;
		case reset: /*done*/
			reset_command();
			break;
		case exits: /*done*/
			exit_command();
			free_Input();
			return;
		}
		reset_input();
	}
}

