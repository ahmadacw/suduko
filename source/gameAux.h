/*
 * gameAux.h
 *
 *  Created on: Apr 10, 2020
 *      Author: pc
 */

#ifndef GAMEAUX_H_
#define GAMEAUX_H_

/* general use */
void empty_checked();
int check_error(int i, int j, int val);
void update_err();
int erroneous();
void update_numOfFilledCells();
void markAllfixd();
void free_matrix(int** matrix,int size);
void empty_sol();

/* board related functions */
void free_board();
void initiate_board();
int malloc_game();
void free_board();
void printSeparator();

/* generate functions */
int fill_x_random_cells(int x,int** lst);
void choose_Y_random_cells(int Y);

/* save load functions */
int loadgameBoard(char* FileName);

#endif /* GAMEAUX_H_ */
