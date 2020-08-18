/*
 * linkedListStrategy.h
 *
 *  Created on: Apr 10, 2020
 *      Author: pc
 */

#ifndef LINKEDLISTSTRATEGY_H_
#define LINKEDLISTSTRATEGY_H_

/* dont forget to delete it*/
typedef enum {
	Undo, Redo
} Control;

typedef struct node{
	struct node* next;
	struct node* prev;
	/*arr[i][0]=row of node i, arr[i][1]= column of node i
	 * , arr[2]=old value of node i*/
	int  **arr;
	int size;/* size of array, == number of cells changed during a certain action*/
}node;

typedef struct list{
	node* first;
	node* curr;
}list;

struct list* myList;

/* linkedList functuons*/
void update_list(int CellsChanged, int *** matrix);
int move_forward();
int move_back();
void init_list();
void free_list();
void getMove(int index);
int noNext();
int noPrev();

#endif /* LINKEDLISTSTRATEGY_H_ */
