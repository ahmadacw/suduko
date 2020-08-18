
#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "Gurobi.h"
#include "gameAux.h"
#include "gurobi_c.h"

#define N n*m
#define size N*N*N
GRBenv *env = NULL;
GRBmodel *model = NULL;
int *ind; /*the indexes of the variables to use in a constrian (size of n*m)*/
double *val;/*the coefficients (according to variables in "ind")*/
double *obj;/*the coeffs of the function that we want to maximize(size of (n*m)^3)*/
double *sol;
char *vtype;/*GRB_BINARY*/
/*
 * the variabls are constructed like this :-
 * [X(i,j,0),X(i,j+1,0)...X(i+1,j,0)...X(i,j,1),X(i,j+1,1)...]
 * formula for X(i,j,k)=i*(m)+j+m*n*(k-1)
 */
void allocArrays() {
	varHash = (int*) calloc(size, sizeof(int));
	obj = (double*) calloc(size, sizeof(double));
	sol = (double*) malloc(size * sizeof(double));
	val = (double*) malloc(size * sizeof(double));
	vtype = (char*) malloc(size * sizeof(char));
	ind = (int*) calloc(size, sizeof(int));
	ind[size - 1] = -1;
}

void resetInd() {
	int i = 0;
	while (ind[i] != -1) {
		ind[i] = -1;
		i++;
	}
}
void init_vars(int l) {
	int i;
	if (!l) {
		for (i = 0; i < size; i++) {

			val[i] = 1.0;
			vtype[i] = GRB_BINARY;

		}
	} else {
		for (i = 0; i < size; i++) {
			val[i] = 1.0;
			vtype[i] = GRB_CONTINUOUS;
		}
	}
}

int setCells() {
	int i, j, error;
	error = 0;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (gameBoard[i][j].cur != 0
					&& varHash[calcIndx(i, j, gameBoard[i][j].cur)] != -1) {
				resetInd();
				ind[0] = varHash[calcIndx(i, j, gameBoard[i][j].cur)];
				if (ind[0] != -1) {
					error = GRBaddconstr(model, 1, ind, val, GRB_EQUAL, 1.0,
					NULL);
				}

				if (error) {
					printf("ERROR %d GRBaddvars(): %s\n", error,
							GRBgeterrormsg(env));

					return 0;
				}
			}
		}
	}
	return 1;
}
int forceCell() {
	int rowIndx, colIndx, i, error, idx;
	error = 0;
	for (rowIndx = 0; rowIndx < N; rowIndx++) {
		for (colIndx = 0; colIndx < N; colIndx++) {
			if (gameBoard[rowIndx][colIndx].cur == 0) {
				resetInd();
				idx = 0;
				for (i = 1; i <= N; i++) { /*for each value*/
					if (varHash[calcIndx(rowIndx, colIndx, i)] != -1) {
						ind[idx] = varHash[calcIndx(rowIndx, colIndx, i)];
						idx++;
					}
				}
				if (ind[0] != -1) {
					error = GRBaddconstr(model, idx, ind, val, GRB_EQUAL, 1.0,
					NULL);
				}

				if (error) {
					printf("ERROR %d GRBaddvars(): %s\n", error,
							GRBgeterrormsg(env));

					return 0;
				}

			}
		}
	}
	return 1;
}
int calcIndx(int i, int j, int k) {
	return (i * N + j + N * N * (k - 1));
}
/* unique values in each cell of a block*/
int updateblock() {
	int rowIndx, colIndx, i, j, x, y, k, idx, error;
	error = 0;
	for (rowIndx = 0; rowIndx < m; rowIndx++) {
		x = rowIndx * n;
		for (colIndx = 0; colIndx < n; colIndx++) { /*for each block*/
			y = colIndx * m;
			for (i = 1; i < N + 1; i++) { /* for each value*/
				resetInd();
				idx = 0;
				for (j = 0; j < n; j++) { /* for each cell*/
					for (k = 0; k < m; k++) {
						if (varHash[calcIndx(x + j, k + y, i)] != -1) {
							ind[idx] = varHash[calcIndx(x + j, k + y, i)];
							idx++;
						}
					}
				}
				if (ind[0] != -1) {
					error = GRBaddconstr(model, idx, ind, val, GRB_EQUAL, 1.0,
					NULL);
				}

				if (error) {
					printf("ERROR %d GRBaddvars(): %s\n", error,
							GRBgeterrormsg(env));

					return 0;
				}
			}
		}
	}
	return 1;
}

int updatecol() {
	int rowIndx, colIndx, k, idx, error;
	error = 0;
	for (colIndx = 0; colIndx < N; colIndx++) { /* for each column*/
		for (k = 1; k < N + 1; k++) { /* for each number*/
			resetInd();
			idx = 0;
			for (rowIndx = 0; rowIndx < N; rowIndx++) { /* for each cell*/
				if (varHash[calcIndx(rowIndx, colIndx, k)] != -1) {
					ind[idx] = varHash[calcIndx(rowIndx, colIndx, k)];
					idx++;
				}
			}
			if (ind[0] != -1) {
				error = GRBaddconstr(model, idx, ind, val, GRB_EQUAL, 1.0,
				NULL);
			}

			if (error) {
				printf("ERROR %d GRBaddvars(): %s\n", error,
						GRBgeterrormsg(env));

				return 0;
			}
		}
	}
	return 1;
}
int updaterow() {
	int rowIndx, colIndx, k, idx, error;
	error = 0;
	for (rowIndx = 0; rowIndx < N; rowIndx++) { /* for each row*/
		for (k = 1; k < N + 1; k++) { /*for each number*/
			resetInd();
			idx = 0;
			for (colIndx = 0; colIndx < N; colIndx++) { /*for each cell*/
				if (varHash[calcIndx(rowIndx, colIndx, k)] != -1) {

					ind[idx] = varHash[calcIndx(rowIndx, colIndx, k)];
					idx++;
				}
			}
			if (ind[0] != -1) {
				error = GRBaddconstr(model, idx, ind, val, GRB_EQUAL, 1.0,
				NULL);
			}

			if (error) {
				printf("ERROR %d GRBaddvars(): %s\n", error,
						GRBgeterrormsg(env));

				return 0;
			}
		}
	}
	return 1;
}
void freeAlmostAll() {
	GRBfreemodel(model);
	GRBfreeenv(env);
	free(obj);
	free(val);
	free(ind);
	free(vtype);
}
void freeAll() {
	GRBfreemodel(model);
	GRBfreeenv(env);
	free(obj);
	free(val);
	free(ind);
	free(vtype);
	free(sol);
	free(varHash);
}

int countLegal(int i, int j) {
	int count, k;
	count = 0;
	for (k = 1; k < N + 1; k++) {
		count += check_error(i, j, k);
	}
	return N - count;
}

void countVars(int *count) {
	int i, j, k;
	for (k = 0; k < N; k++) {
		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {
				if ((gameBoard[i][j].cur != 0) | (check_error(i, j, k + 1))) {
					varHash[calcIndx(i, j, k + 1)] = -1;
				} else {
					obj[(*count)] =  countLegal(i, j);
					varHash[calcIndx(i, j, k + 1)] = ((*count)++);
				}
			}

		}
	}
}

void fillBoardWithSol() {
	int rowInd, colInd, k;
	for (rowInd = 0; rowInd < N; rowInd++) {
		for (colInd = 0; colInd < N; colInd++) {
			if(gameBoard[rowInd][colInd].cur!=0) continue;
			for (k = 0; k < N; k++) {
				if (sol[varHash[calcIndx(rowInd, colInd, k + 1)]]==1) {
					gameBoard[rowInd][colInd].sol = k + 1;
					break;
				}
			}
		}
	}
}

int ILP(int type) {
	int v1, v2, v3, v4, v5;
	int error = 0;
	int optimstatus;


	int varCount, solvable;
	solvable = 0;
	(varCount) = 0;

	allocArrays();
	init_vars(type);
	countVars(&varCount);
	/* Create environment - log file is mip1.log */
	error = GRBloadenv(&env, "mip1.log");
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));

		printf("coudln't load env\n");
		freeAll();
		return -1;
	}

	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
		printf("probelm at setintparam ");
		freeAll();
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* Create an empty model named "mip1" */
	error = GRBnewmodel(env, &model, "mip1", 0, NULL, NULL, NULL, NULL, NULL);
	if (error) {
		printf("couldn't create new model ");
		freeAll();
		printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
		return -1;
	}

	/* add variables to model */
	if (type == 1) {
		/* add variables to model */
		error = GRBaddvars(model, varCount, 0, NULL, NULL, NULL, obj, NULL,
		NULL, vtype, NULL);
		if (error) {
			printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));

			printf("couldn't add vars\n");
			freeAll();
			return -1;
		}
		/* Change objective sense to maximization */
		error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
		if (error) {
			printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));

			printf("couldn't set to max\n");
			freeAll();
			return -1;
		}
	} else {
		/* add variables to model */
		error = GRBaddvars(model, varCount, 0, NULL, NULL, NULL, obj, NULL,
		NULL, vtype, NULL);
		if (error) {
			printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
			printf("couldn't add variables to model\n");
			freeAll();
			return -1;
		}

	}

	/* update the model - to integrate new variables */

	error = GRBupdatemodel(model);
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));

		printf("couldn't update model\n");
		return -1;
	}
	v1 = setCells();
	v2 = forceCell();
	v3 = updateblock();
	v4 = updatecol();
	v5 = updaterow();
	if (!(v1 && v2 && v3 && v4 && v5)) {
		return -1;
	}

	/* Optimize model - need to call this before calculation */
	error = GRBoptimize(model);

	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		printf("couldn't optimize\n");
		freeAll();
		return -1;
	}
	error = GRBwrite(model, "mip1.lp");
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		printf("couldn't write\n");
		freeAll();
		return -1;
	}

	/* Get solution information */
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		printf("couldn't get solution information\n");
		freeAll();
		return 0;
	}

	/* get the solution - the assignment to each variable */

	/* solution found */
	if (optimstatus == GRB_OPTIMAL) {
	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, varCount, sol);
		if (error) {
			freeAll();
			return 0;
		}
		if (type == 0) {
			fillBoardWithSol();
			printBoard();
			freeAll();
			return 1;
		}
		freeAlmostAll();
		return 1;
	}

	/* no solution found */
	else{
		freeAll();
		return 0;
	}
	/* IMPORTANT !!! - Free model and environment */
	freeAlmostAll();
	return solvable;
}

