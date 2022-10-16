///
/// File:  matrix.h
/// Description:  Defines functions for matrix functions and structs
///               used in the CSCI-243 scanner project
///
/// @author lpa6230@rit.edu

#ifndef MATRIX_H_
#define MATRIX_H_

#define BUFLENGTH 256

typedef struct {
    int state;
    int discard;
} destination;

void createEmptyMatrix(destination **arr, int rows, int cols);

void matrixInsert(destination dest, destination *arr, int row, int col);

destination getMatrix(destination *arr, int row, int col);

void generateTM(destination **tm, int *numStates, int *startState, int *acceptState, char *fileDir);

void printMatrix(destination *tm, int numStates);

#endif
