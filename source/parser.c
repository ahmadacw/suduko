/*
 * parser.c
 *
 *  Created on: Apr 10, 2020
 *      Author: pc
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "game.h"

/* reset inp*/
void reset_input() {
	int i;
	inp->command = exits;
	inp->fault = 0;
	for (i = 0; i < 256; i++) {
		inp->second[i] = '\0';
		inp->fourth[i] = '\0';
		inp->third[i] = '\0';
		inp->first[i] = '\0';
	}
}

/*free memory of inp*/
void free_Input() {
	free(inp->second);
	free(inp->third);
	free(inp->fourth);
	free(inp->first);
	free(inp);
}

/*initilaize inp*/
void init_input() {
	inp = (input*) malloc(sizeof(input));
	inp->first = (char*) malloc(sizeof(char) * 256);
	inp->second = (char*) malloc(sizeof(char) * 256);
	inp->third = (char*) malloc(sizeof(char) * 256);
	inp->fourth = (char*) malloc(sizeof(char) * 256);
}

/* filling inp -- receiving input*/
void recieve_Input() {
	init_input();
	char c;
	int i = 0, j = 0, cnt = 0;
	reset_input();
	do {
		c = getchar();
	} while (c == '\t' || c == '\r' || c == ' ');

	inp->fault = 0;/*assuming that it's a valid command*/
	while (i < 4 && c != EOF && c != '\n') {
		j = 0;
		while (c != EOF) {
			if (cnt == 256) {
				while (c != EOF && c != '\n')
					c = getchar();
				return;
			}
			if (c == '\t' || c == '\r' || c == '\n' || c == ' ') {
				cnt++;
				break;
			}
			switch (i) {
			case (0): {
				inp->first[j] = c;
				break;
			}
			case (1): {
				inp->second[j] = c;
				break;
			}
			case (2): {
				inp->third[j] = c;
				break;
			}
			case (3): {
				inp->fourth[j] = c;
				break;
			}
			}
			j++;
			cnt++;
			c = getchar();
		}
		switch (i) {
		case (0): {
			inp->first[j] = '\0';
			break;
		}
		case (1): {
			inp->second[j] = '\0';
			break;
		}
		case (2): {
			inp->third[j] = '\0';
			break;
		}
		case (3): {
			inp->fourth[j] = '\0';
			break;
		}
		}
		while (c != '\n' && c != EOF && (c == '\t' || c == '\r' || c == ' ')) {
			c = getchar();
		}
		i++;
	}
	while (c != EOF && c != '\n') {
		c = getchar();
		if (c != '\t' && c != '\r' && c != ' ')
			inp->fault = 1;
		cnt++;
	}
	if (cnt >= 256) {
		printf("Too many Charachters entered in a single line.");
		inp->command = invalid;
		return;
	}
}

/*updating the proper command*/
com ConvertInput() {
	if (inp->fault != 1) {
		if (strcmp("solve", inp->first) == 0)
			return solve;
		else if (strcmp("edit", inp->first) == 0)
			return edit;
		else if (strcmp("mark_errors", inp->first) == 0)
			return mark_errors;
		else if (strcmp("print_board", inp->first) == 0)
			return print_board;
		else if (strcmp("set", inp->first) == 0)
			return set;
		else if (strcmp("validate", inp->first) == 0)
			return validate;
		else if (strcmp("guess", inp->first) == 0)
			return guess;
		else if (strcmp("generate", inp->first) == 0)
			return generate;
		else if (strcmp("hint", inp->first) == 0)
			return hint;
		else if (strcmp("guess_hint", inp->first) == 0)
			return guess_hint;
		else if (strcmp("undo", inp->first) == 0)
			return undo;
		else if (strcmp("redo", inp->first) == 0)
			return redo;
		else if (strcmp("save", inp->first) == 0)
			return save;
		else if (strcmp("num_solutions", inp->first) == 0)
			return num_solutions;
		else if (strcmp("autofill", inp->first) == 0)
			return autofill;
		else if (strcmp("reset", inp->first) == 0)
			return reset;
		else if (strcmp("exit", inp->first) == 0)
			return exits;
		return invalid;
	}
	return inp->command;
}
/* checks if input is integer*/
int check_int(char *input) {
	while (input[0] != '\0') {
		if (!isdigit(input[0]))
			return 0;
		input++;
	}
	return 1;
}

void check_Input() {
	inp->command = ConvertInput();

	if (inp->command != invalid && inp->fault == 1) {
		printf("Too many parameters, ");
		inp->command = invalid;
		return;
	}
	if ((inp->command == print_board || inp->command == validate
			|| inp->command == undo || inp->command == redo
			|| inp->command == num_solutions || inp->command == autofill
			|| inp->command == reset) && *inp->second != '\0') {
		printf("Too many parameters, ");
		inp->command = invalid;
	}
	if (inp->command == save || inp->command == solve || inp->command == edit
			|| inp->command == mark_errors || inp->command == guess) {
		if (inp->third[0] != '\0') {
			printf("Too many parameters, ");
			inp->command = invalid;
		} else if ((inp->command == save || inp->command == guess
				|| inp->command == solve) && inp->second[0] == '\0') {
			printf("Too few parameters, ");
			inp->command = invalid;
		}

	}
	if (inp->command == mark_errors) {
		if (*inp->second == '\0') {
			printf("Too few parameters, ");
			inp->command = invalid;
		}
		if (!check_int(inp->second)) {
			printf("Value is not integer, ");
			inp->command = invalid;
		}
		if (inp->second[1] != '\0'
				|| (atoi(inp->second) != 0 && atoi(inp->second) != 1)) {
			printf("\n%d", atoi(inp->second));
			printf("Value can only be 0 or 1, ");
			inp->command = invalid;
		}
	}
	if (inp->command == set || inp->command == hint
			|| inp->command == guess_hint) {
		if ((inp->command == hint || inp->command == guess_hint)
				&& *(inp->fourth) != '\0') {
			printf("Too many parameters, ");
			inp->command = invalid;
		} else if (*(inp->second) == '\0' || *(inp->third) == '\0'
				|| (inp->command == set && *(inp->fourth) == '\0')) {
			printf("Too few parameters, ");
			inp->command = invalid;
		} else if (!check_int(inp->second)) {
			printf("First parameter is not integer, ");
			inp->command = invalid;
		} else if (!check_int(inp->third)) {
			printf("Second parameter is not integer, ");
			inp->command = invalid;
		} else if (inp->command == set && !check_int(inp->fourth)) {
			printf("Third parameter is not integer, ");
			inp->command = invalid;
		} else if (atoi(inp->second) < 1
				|| (currMode != INIT && atoi(inp->second) > n * m)) {
			printf("First parameter is out of range, ");
			inp->command = invalid;
		} else if (atoi(inp->third) < 1
				|| (currMode != INIT && atoi(inp->third) > n * m)) {
			printf("Second parameter is out of range, ");
			inp->command = invalid;
		} else if (inp->command == set) {
			if (atoi(inp->fourth) < 0
					|| (currMode != INIT && atoi(inp->fourth) > n * m)
					|| (*(inp->fourth) != '0' && atoi(inp->fourth) == 0)) {
				printf("Third parameter is out of range, ");
				inp->command = invalid;
			}
		} else if (currMode == SOLVE) {
			if (atoi(inp->second) > n * m)
				printf("First parameter is out of range, Invalid command.\n");
			else if (atoi(inp->third) > n * m)
				printf("second parameter is out of range, Invalid command.\n");
		}
	}
	if (inp->command == generate) {
		if (*inp->fourth != '\0') {
			printf("Too many parameters, ");
			inp->command = invalid;
		} else if (*inp->second == '\0' || *inp->third == '\0') {
			printf("Too few parameters, ");
			inp->command = invalid;
		} else if (!check_int(inp->second)) {
			printf("First parameter is not integer, ");
			inp->command = invalid;
		} else if (!check_int(inp->third)) {
			printf("Second parameter is not integer, ");
			inp->command = invalid;
		} else if (currMode != INIT) {
			if (atoi(inp->second) < 0 || atoi(inp->second) > n * m * n * m) {
				printf("First parameter is out of range, ");
				inp->command = invalid;
			} else if (atoi(inp->third) < 0
					|| (*inp->third != '0' && atoi(inp->third) == 0)
					|| atoi(inp->third) > n * m * n * m) {
				printf("Second parameter is out of range, ");
				inp->command = invalid;
			}
		}
	}
}

