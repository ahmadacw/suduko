/*
 ============================================================================
 Name        : ProjectMain.c
 Author      : pc
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include "game.h"


int main(void) {
	setbuf(stdout,NULL);
	srand(3);
	startGame();
	return EXIT_SUCCESS;
}
