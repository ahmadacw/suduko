/*
 * parser.h
 *
 *  Created on: Apr 10, 2020
 *      Author: pc
 */

#ifndef PARSER_H_
#define PARSER_H_

typedef enum {
	invalid,
	solve,
	generate,
	hint,
	reset,
	mark_errors,
	autofill,
	num_solutions,
	save,
	edit,
	exits,
	validate,
	guess_hint,
	guess,
	undo,
	print_board,
	redo,
	set,

} com;


typedef struct input {
	int fault;/* incorrect input */
	com command;/* current command stored here*/
	char* first;/*stores the command before interpreting it*/
	char* second;/* first argument*/
	char* third;/*second argument*/
	char* fourth;/*third argument*/
}input;

struct input *inp;

/* functions */
void free_Input();
void recieve_Input();
void check_Input();
void reset_input();

#endif /* PARSER_H_ */
