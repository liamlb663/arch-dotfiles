///
/// File:  scanner.h
/// Description:  Defines functions for matrix functions and structs
///               used in the CSCI-243 scanner project
///
/// @author lpa6230@rit.edu

#ifndef SCANNER_H_
#define SCANNER_H_

#include "matrix.h"

#define TOKENLEN 80

void scanInput(destination *tm, int state, int start, int accept);

#endif
