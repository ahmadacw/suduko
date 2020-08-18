/*
 * stack.h
 *
 *  Created on: Apr 10, 2020
 *      Author: pc
 */

#ifndef STACK_H_
#define STACK_H_

typedef struct Stack {
	int curr_size;
	struct Link* top;
} Stack;
/*the data member of the stack*/
typedef struct Link {
	int row, col;
	struct Link* next;
} Link;
/*initializes the stack*/
Stack* initStack();
/*checks if the stack is empty*/
int is_Empty(Stack* stack);
/*push to stack the values i j*/
void push(int i,int j,struct Stack * stack);
/*pop from stack the values i j*/
void pop(int* i , int* j,struct Stack* stack);
/*peeks at the top of the stack*/
void top(int* i, int* j,struct Stack* stack);
/*frees all memory corresponding with the stack*/
void freeStack(Stack* stack);

#endif /* STACK_H_ */
