all: sudoku-console
CC = gcc
OBJS = ProjectMain.o gameAux.o  parser.o game.o  linkedListStrategy.o  stack.o  Commands.o backtracking.o Gurobi.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra \
-Werror -errors -g
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o  $@
Commands.o: Commands.c Commands.h linkedListStrategy.h game.h gameAux.h backtracking.h Gurobi.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
parser.o: parser.c parser.h game.h
	$(CC) $(COMP_FLAG) -c $*.c
ProjectMain.o: ProjectMain.c game.h 
	$(CC) $(COMP_FLAG) -c $*.c
backtracking.o: backtracking.c backtracking.h stack.h game.h
	$(CC) $(COMP_FLAG) -c $*.c
gameAux.o: gameAux.c gameAux.h game.h Commands.h linkedListStrategy.h
	$(CC) $(COMP_FLAG) -c $*.c
game.o: game.c game.h gameAux.h parser.h backtracking.h Commands.h
	$(CC) $(COMP_FLAG) -c $*.c
linkedListStrategy.o: linkedListStrategy.c linkedListStrategy.h gameAux.h
	$(CC) $(COMP_FLAG) -c $*.c
Gurobi.o: Gurobi.c Gurobi.h game.h gameAux.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c
stack.o: stack.c stack.h
	$(CC) $(COMP_FLAG) -c $*.c	
clean:
	rm -f $(OBJS) $(EXEC)
