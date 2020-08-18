/*
 * ILP.h
 *
 *  Created on: 30 Mar 2020
 *      Author: laptop
 */

#ifndef GUROBI_H_
#define GUROBI_H_
int* varHash;
double *sol;

int ILP(int type);
int calcIndx(int i ,int j, int k);

#endif /* GUROBI_H_ */
