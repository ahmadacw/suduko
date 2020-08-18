/*
 * Commands.h
 *
 *  Created on: Apr 10, 2020
 *      Author: pc
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

/* functionalities*/
int set_f(int x, int y, int z);
void hint_f(int x, int y);
void undo_f();
void redo_f();
int validateBoard();
int generate_f(int x, int y);
int loadDefault();
void guess_f();
void autoFill_f();
void guessHint_f(int i, int j);
int savegameBoard(char* FileName, int index);
void printBoard();
void reset_board();


#endif /* COMMANDS_H_ */
