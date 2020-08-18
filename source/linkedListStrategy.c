/*
 * linkedListStrategy.c
 *
 *  Created on: Apr 10, 2020
 *      Author: pc
 */

#include <stdio.h>
#include <stdlib.h>
#include "linkedListStrategy.h"
#include "gameAux.h"

/* init and malloc nodes*/
node* init_node(){
	node* new = (node*) malloc(sizeof(node));
	new->next=0;
	new->prev=0;
	new->size=0;
	new->arr=NULL;
	return new;
}

/* malloc and init memory of myList*/
void init_list() {
	node* new = init_node();
	myList = (struct list*) malloc(sizeof(struct list));
	myList->curr=new;
	/*can't go back beyond first cell(empty cell
	 * look shape at end of this function)*/
	myList->curr->prev=myList->curr;
	myList->first=myList->curr;

}/*  empty Cell <---> Cell(move 1) <---> Cell(move 2) <---> Cell(move 3)...*/

int noPrev() {
	return myList->curr->size== 0;/*reached the empty Cell */
}

int noNext() {
	return myList->curr->next == 0;
}

/*prev node*/
int move_back() {
	if (noPrev()){
		return 0;
	}
	myList->curr = myList->curr->prev;
	return 1;
}

/*next node*/
int move_forward() {
	if (noNext())
		return 0;
	myList->curr = myList->curr->next;
	return 1;
}

/* delete all nodes from current to last in myList*/
void freeToLast() {
	node* tmp = 0;
	node* tmp1 = 0;
	int i;
	if (myList->curr->next != 0){
		tmp = myList->curr->next;
		myList->curr->next = NULL;
		while (tmp) {
			for (i = 0; i < tmp->size; i++)
				free(tmp->arr[i]);
			tmp1 = tmp->next;
			free(tmp);
			tmp = tmp1;
		}
	}
}

/* concatenate*/
void concatenate(node* new) {
	new->prev = myList->curr;
	new->next=myList->curr->next;
	myList->curr->next = new;
	myList->curr=new;
}

/*concatenate matrix(arr) accordingly*/
void update_list(int CellsChanged, int *** matrix) {
	node* new = init_node();
	freeToLast();
	concatenate(new);
	new->arr = *matrix;
	new->size = CellsChanged;
}

/* free memory of myList*/
void free_list() {
	int i;
	node* tmp;
	while ((myList->first->next)!=NULL) {
		for (i = 0; i < myList->first->next->size; i++){
			free(myList->first->next->arr[i]);
		}
		tmp=myList->first->next;
		myList->first->next=tmp->next;
		tmp->prev=tmp;
		free(tmp);
	}
	free(myList);
}

/*index=0 undo, index =1 redo*/
void getMove(int index) {/* we are currently pointing at the last move made*/
	if (index==0) {
		move_back();
		return;
	}
	if (index) {
		move_forward();
		return;
	}
}

