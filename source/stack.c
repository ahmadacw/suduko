/*
 * stack.c
 *
 *  Created on: Apr 10, 2020
 *      Author: pc
 */
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

Stack* initStack(){
	Stack* stack= (Stack*) malloc(sizeof(Stack));
	stack->curr_size=0;
	stack->top=0;
	return stack;
}

int is_Empty(Stack* stack){
	if((*stack).curr_size==0) return 1;
	return 0;
}

void push(int i,int j,struct Stack * stack){
	Link* new_top= (Link*) malloc(sizeof(Link));
	new_top->row=i;
	new_top->col=j;
	new_top->next=(*stack).top;
	(*stack).top=new_top;
	(*stack).curr_size++;
}

void pop(int* i , int* j,struct Stack* stack){
	if((*stack).curr_size<=0){
		printf("stack is empty, you have a problem");
		*i=-1;
		return ;
	}
	(*stack).curr_size--;
	(*i)=stack->top->row;
	(*j)=stack->top->col;
	Link* top = (*stack).top;
	stack->top=top->next;
	free(top);

}

void top(int* i, int* j,Stack* stack){
	*i=stack->top->row;
	*j=stack->top->col;
}

void freeStack(Stack* stack){
	int i,j;
	i=0;
	j=0;
	while(stack->curr_size>0){
		pop(&i,&j,stack);
	}
	free(stack);
}

