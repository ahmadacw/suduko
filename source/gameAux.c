/*
 * gameAux.c
 *
 *  Created on: Apr 10, 2020
 *      Author: pc
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "linkedListStrategy.h"
#include "gameAux.h"
#include "game.h"
#include "Commands.h"

#define N (n*m)

int** helpArray;
int max_length;/* max length for load func*/

/* checking board validity for fixd cells (for solve x)*/
int check_fixed_validity(CELL** tmp);

void empty_checked() {
	int row, col;
	for (row = 0; row < N; row++) {
		for (col = 0; col < N; col++) {
			gameBoard[row][col].checked = 0;
		}
	}
}

/*checks if val is already in row j*/
int sameRow(int i, int j, int val) {
	int k;
	for (k = 0; k < N; k++) {
		if (val == gameBoard[k][j].cur && k!=i){
			return 1;
		}
	}
	return 0;
}

/*checks if val is already in col i*/
int sameCol(int i, int j, int val) {
	int k;
	for (k = 0; k < N; k++) {
		if (val == gameBoard[i][k].cur && k!=j){
			return 1;
		}
	}
	return 0;
}

/*checks if val is already exists in this block*/
int sameBlock(int i, int j, int val) {
	int currGridx, currGridy, k=0, r;
	while (i >= k * (n))
		k++;
	currGridx = (k - 1) * n;
	k = 0;

	while (j >= k * (m))
		k++;
	currGridy = (k - 1) * m;
	for (k = currGridx; k < (currGridx + n); k++) {
		for (r = currGridy; r < (currGridy + m); r++) {
				if (gameBoard[k][r].cur == val &&( k!=i || r!=j)){
					/* second condition == not the same cell */
					return 1;
			}
		}
	}
	return 0;
}

void empty_sol() {
	int i, j;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			gameBoard[i][j].sol = 0;
		}
	}
}

int check_error(int i, int j, int val) {
	return sameBlock(i, j, val) || sameRow(i, j, val) || sameCol(i, j, val);
}

/*function for updating gameBoard accordingly*/
void update(int row, int col, int val,int fixed,CELL** gameBoard) {
	gameBoard[row][col].cur = val;
	gameBoard[row][col].fixd = fixed;
}

void update_err(){
	int row, col ;
		for (row = 0; row < n * m; row++)
				for (col = 0; col < n * m; col++) {
					if(gameBoard[row][col].cur != 0)
						gameBoard[row][col].err=check_error(row,col,gameBoard[row][col].cur);
					else gameBoard[row][col].err=0;
				}
}

/* return 1 if board is erroneous*/
int erroneous(CELL** gameBoard) {
	int i, j;
	for (i = 0; i < n * m; i++)
		for (j = 0; j < n * m; j++) {
			if (gameBoard[i][j].err == 1){
				return 1;}
		}
	return 0;
}

void update_numOfFilledCells() {
	int i, j;
	numOfFilledCells = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (gameBoard[i][j].cur != 0)
				numOfFilledCells += 1;
		}
	}
}

void markAllfixd() {
	int j, i;
	for (j = 0; j < n * m; j++)
		for (i = 0; i < n * m; i++) {
			if (gameBoard[j][i].cur != 0)
				gameBoard[j][i].fixd = 1;
		}
}

/* function to free memory of matrixes*/
void free_matrix(int** matrix,int size){/* general use*/
	int i;
	for(i=0;i<size;i++){
		free(matrix[i]);
	}
	free(matrix);
}
/*initiates board to zeros*/
void initiate_board() {
	int row, col;
	for (row = 0; row < N; row++)
		for (col = 0; col < N; col++) {
			gameBoard[row][col].cur = 0;
			gameBoard[row][col].fixd = 0;
			gameBoard[row][col].err = 0;
			gameBoard[row][col].sol = 0;
			gameBoard[row][col].vald = 0;
			gameBoard[row][col].checked = 0;
		}
	game_already_created = 1;
	numOfFilledCells=0;
}

/* free memory of current board*/
void free_board() {
	int i;
	free_list();
	for (i = 0; i < N; i++) {
		free(gameBoard[i]);
	}
	free(gameBoard);
	game_already_created = 0;
	numOfFilledCells = 0;
}
/* malloc board*/
int malloc_game() {
	int i;
	if (game_already_created == 1)
		free_board();

	gameBoard = ((CELL **) malloc((N * N)* sizeof(CELL*)));
	if (gameBoard==NULL){
		printf("failed to initiate.\n");
		return 0;
	}
	for (i = 0; i < N; i++) {
		gameBoard[i] = ((CELL*) malloc(N*sizeof(CELL)));
		if (gameBoard[i]==NULL){
			free_board();
			printf("failed to initiate.\n");
			return 0;
		}
	}
	init_list();
	return 1;
}

/* for load function use */
void free_copy(CELL** tmp, int size){
	int i;
	for(i=0;i<size;i++){
		free(tmp[i]);
	}
	free(tmp);
}

/* init and malloc of a board copy*/
CELL** init_copy() {
	int i;
	int row, col;
	CELL** tmp;
	tmp = ((CELL **) malloc(N * sizeof(CELL *)));
	if (tmp == NULL) {
		printf("failed to initiate.\n");
		return NULL;
	}
	for (i = 0; i < N; i++) {
		tmp[i] = ((CELL*) malloc(N* sizeof(CELL)));
		if (tmp[i] == NULL) {
			free_copy(tmp,i);
			printf("failed to initiate.\n");
			return NULL;
		}
	}
	for (row = 0; row < N; row++){
		for (col = 0; col < N; col++) {
			tmp[row][col].cur = 0;
			tmp[row][col].fixd = 0;
			tmp[row][col].err = 0;
			tmp[row][col].sol = 0;
			tmp[row][col].vald = 0;
			tmp[row][col].checked = 0;
		}
	}
	return tmp;
}

/* -------------------------------------- end of general functions--------------------------------*/

/* for generate function use */
int init_helpArray() {
	int i;
	helpArray = ((int**) malloc(N*N-numOfFilledCells * sizeof(int*)));
	if (!helpArray) {
		return 0;
	}
	for (i = 0; i <N*N-numOfFilledCells; i++) {
		helpArray[i] = ((int*) malloc(2 * sizeof(int)));
		if (!helpArray[i]) {
			free_matrix(helpArray,N*N);
			return 0;
		}
	}
	return 1;
}

/*index=0 for fill x cells function, index=1 for fill y cells function
 * each cell contains two indexes */
int helpArrayFunc(int index) {
	int i, j, k = 0;
	if(!init_helpArray()){
		return 0;
	}
	if (index == 0) {
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				if (gameBoard[i][j].cur == 0) {
					helpArray[k][0] = i;
					helpArray[k][1] = j;
					++k;
				}
			}
		}
	} else {
		k=0;
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				if (gameBoard[i][j].checked == 0) {
					helpArray[k][0] = i;
					helpArray[k][1] = j;
					++k;
			}
		}
	}
	}
	return 1;
}
/* -------------------------------  generate function ------------------------------ */

/* k is the number of cells we want to "avoid".
 * returns indexes of a random cell in Board */
int* chooseRandomCell(int gridSize, int k) {
		int rCell = rand() % (gridSize - k);
		return helpArray[rCell];
}

/* valid values for gameBoard[row][col]*/
int validVals(int row, int col,int* vals) {
	int k,length=0;/* length== how much unerroneous values we have*/
	for (k = 0; k < N; k++) {
		if (!check_error(row, col, k + 1)){
			vals[length] = k + 1;
			++length;
		}
	}
	return length;

}

int* init_vals(){
	int* vals=(int*)calloc(N,sizeof(int));
	if (!vals){
		free(vals);
		return NULL;
	}
	return vals;
}

int fill_x_random_cells(int x,int** lst) {
	int i,length,value;
	int* index=0;
	int* vals=0;

	for (i = 0; i < x; i++) {
		vals=init_vals();
		if(!vals){
			return 2;/* failed to allocate memory index*/
		}
		if(!helpArrayFunc(0)){
			free(vals);
			free_matrix(lst,N);
			return 2;
		}
		index = chooseRandomCell(N * N, numOfFilledCells);
		length=validVals(index[0],index[1],vals);
		if (length == 0) {
			free(vals);
			free_matrix(lst,N);
			free_matrix(helpArray,N*N);
			return 0;
		}
		value = vals[rand()%length];
		gameBoard[index[0]][index[1]].cur = value;
		free(vals);
		free_matrix(helpArray,N*N-numOfFilledCells);
		}
	return 1;
}

void choose_Y_random_cells(int Y) {
	int i, j;
	int* index;
	empty_checked();
	numOfFilledCells=0;
	for (i = 0; i < Y; i++) {
		helpArrayFunc(1);
		index = chooseRandomCell(N * N, i);
		gameBoard[index[0]][index[1]].checked = 1;
		free_matrix(helpArray,N*N-numOfFilledCells);
		numOfFilledCells+=1;
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (gameBoard[i][j].checked){
				gameBoard[i][j].cur = gameBoard[i][j].sol;
				gameBoard[i][j].checked=0;/* empty checked*/
			}
			else gameBoard[i][j].cur=0;
		}
	}
}
/* ------------------------- print related function------------------------------ */
void printSeparator() {
	int i;
	for (i = 0; i < 4 * N + n + 1; i++) {
		printf("-");
	}
	printf("\n");
}
/*---------------------------------------- load functions----------------------------------- */


/* as the name says*/
int ImportantChar(char ch) {
	if ((ch == '\n') || (ch == '\t') || (ch == ' ') || (ch == '\r'))
		return 0;
	return 1;
}

/* move fpointer till important char */
char ImportantRead(FILE* fpointer,char ch) {
	do{
		if(feof(fpointer))
			return '\0';
		ch = fgetc(fpointer);

	} while (!ImportantChar(ch));

	return ch;
}

/*as the name says*/
int initiating_m_n_max_length(FILE* fpointer, char ch) {
	int i=0,k=0;
	if (!isdigit(ch)|| ch-'0'<=0 || feof(fpointer)) {
		printf("unValid File.\n");
		return 0;
	}
	n = ch-'0';
	ch = ImportantRead(fpointer,ch);
	if (!isdigit(ch) || ch-'0'<=0 || feof(fpointer)) {
		printf("unValid File.\n");
		return 0;
	}
	m = ch-'0';
	k=N;
	/* finding max number of chars needed
	 *  to write a legal number at current board*/
	while (k > 0) {
		k = k / 10;
		i++;
	}
	max_length = i + 1;/* counting '.' with max length */
	return 1;
}

/* chars is digit and in range*/
int check_ch(char ch) {
	if (!isdigit(ch) ||  ch-'0' > N || ch-'0'<0) {
		printf("unValid File.\n");
		return 0;
	}
	return 1;
}

/* return the index which contains '.' or
 *  return -1 (there isnt such an index) O.W N+2(error occured)*/
int loadCELL(FILE* fpointer, char* buffer1,char* ch) {
	int i, index;
	i = 1;
	index = -1;
	if (!check_ch(*ch)){
		return N+2;
	}
	buffer1[0] = *ch;
	*ch=fgetc(fpointer);
	if(*ch==EOF){
		buffer1[1]='\0';
		return -1;/* we reached EOF*/
	}
	while(i<max_length){
		if(!(ImportantChar(*ch))){
			buffer1[i]='\0';
			break;
		}
		if (*ch == '.'){
			buffer1[i]=*ch;
			index = i;
			*ch=fgetc(fpointer);
			if(*ch==EOF)
				break;
			if(ImportantChar(*ch)){/* illegal char (words such as 23.4)*/
				printf("unValid File.\n");
				return N+2;
			}
			break;
		}
		if(!check_ch(*ch)){
			return N+2;
		}
		buffer1[i]=*ch;
		i++;
		*ch=fgetc(fpointer);
	}

	/* unvalid file: number saved in file too high for N=n*m or illegal char*/
	if(i==max_length && buffer1[i-1]!= '.' &&
					ImportantChar(buffer1[i-1])){
		printf("unValid File.\n");
		return N+2;
	}
	return index;
}

/* next cell to load*/
void nextCell(int* row, int* col) {
	if (*col == N-1)
		*col = 0, *row+=1;
	else
		*col+=1;
}

/* fill whats left of tmp board with zeros*/
void fill_board(int row, int col,CELL** tmp) {
	for (; row < N; row++) {
		for (; col < N; col++)
			update(row, col, 0,0,tmp);
	}
}
/* load functionality*/
int loadgameBoard(char* FileName) {
	char* buffer1;
	int index, row=0, col=0, N_origin, M_origin;
	char ch=' ';
	FILE* fpointer;
	CELL** tmp=0;

	max_length = 0;
	N_origin=n, M_origin=m;

	fpointer = fopen(FileName, "r");
	if (!fpointer){
		printf("Error: couldn't read the file.\n");
		return 0;
	}
	if(feof(fpointer)){
		printf("Error: file is empty.\n");
		return 0;
	}
	ch=(ImportantRead(fpointer,ch));
	if(!initiating_m_n_max_length(fpointer,ch)){
		return 0;
	}
	tmp=init_copy();
	if(!tmp){
		n=N_origin;
		m=M_origin;
		return 0;
	}
	while (ch!=EOF) {
		/* buffer1 stores words*/
		buffer1 = ((char*) calloc(max_length,sizeof(char)));
		if (buffer1 == NULL) {
			n=N_origin;
			m=M_origin;
			free_copy(tmp,N);
			printf("Failed to allocate memory.\n");
			return 0;
		}
		ch= ImportantRead(fpointer,ch);

		/* numbers in File higher than needed*/
		if(ch!=EOF && row == N && col == 0){
			n=N_origin;
			m=M_origin;
			free_copy(tmp,N);
			printf("unValid File.\n");
			return 0;
		}
		if(ch==EOF)
			break;
		index = loadCELL(fpointer, buffer1,&ch);
		if(index==N+2){
			free(buffer1);
			n=N_origin;
			m=M_origin;
			free_copy(tmp,N);
			return 0;
		}
		if(index!=-1){
			buffer1[index]='\0';
			if(atoi(buffer1)==0){
				/* this cell is fixd and have 0 value */
				free_copy(tmp,N);
				printf("unValid File.\n");
				return 0;
			}
			update(row,col,atoi(buffer1),1,tmp);
			free(buffer1);
		}
		else {
			update(row,col,atoi(buffer1),0,tmp);
			free(buffer1);
		}
		if(ch==EOF)
			break;
		nextCell(&row,&col);
	}
	if (row != N && col != 0)
		fill_board(row, col,tmp);
	if(currMode == SOLVE)
		if(!check_fixed_validity(tmp)){
		free_copy(tmp,N);
		return 0;
	}
	if(game_already_created)
		free_board();
	init_list();
	gameBoard=tmp;
	update_numOfFilledCells();
	update_err();
	game_already_created = 1;
	fclose(fpointer);
	return 1;
}

/*checks if val is already in col j at fixed board copy*/
int sameCol_fixd(int i, int j, int val,CELL** tmp) {
	int k;
	for (k = 0; k < N; k++) {
		if ((tmp[k][i].fixd) && (val == tmp[k][j].cur) && k!=i){
			return 1;
		}
	}
	return 0;
}

/*checks if val is already in row i at fixed board copy*/
int sameRow_fixd(int i, int j, int val,CELL** tmp) {
	int k;
	for (k = 0; k < N; k++) {
		if ((tmp[i][k].fixd) && (val == tmp[i][k].cur) &&  k!=j){
			return 1;
		}
	}
	return 0;
}

/*checks if val is already in block at fixed board copy*/
int sameBlock_fixd(int i, int j, int val,CELL** tmp) {
	int currGridx, currGridy, k=0, r;
	while (i >= k * (n))
		k++;
	currGridx = (k - 1) * n;
	k = 0;

	while (j >= k * (m))
		k++;
	currGridy = (k - 1) * m;
	for (k = currGridx; k < (currGridx + n); k++) {
		for (r = currGridy; r < (currGridy + m); r++) {
				if (tmp[k][r].fixd &&( k!=i || r!=j)){
					if(tmp[k][r].cur==val)
						return 1;
			}
		}
	}
	return 0;
}

/* check if val is erroneous at copy fixed*/
int error_tmp_fixd(int i, int j, int val,CELL** tmp) {
	return sameBlock_fixd(i, j, val,tmp) || sameRow_fixd(i, j, val,tmp) || sameCol_fixd(i, j, val,tmp);
}

/* return if copy board fixed is erroneous*/
int err_fixd(CELL** tmp){
	int row,col;
	for (row = 0; row <N; row++){
		for (col = 0; col < N; col++) {
			if(tmp[row][col].fixd){
				if(error_tmp_fixd(row,col,tmp[row][col].cur,tmp))
					return 1;
			}
		}
	}
	return 0;
}

 /* checking board validity for fixd cells (for solve x)*/
int check_fixed_validity(CELL** tmp){
	if(err_fixd(tmp)){
		printf("unValid File.\n");
		return 0;
	}

	return 1;
}
