#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "classes.h"
#include "scanner.h"
#include "matrix.h"

int main(int argc, char* argv[]){
    //assert argc == 2
    if ( argc != 2 ) {
        fprintf(stderr, "usage: %s tmfile\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    destination *tm;
    int states = -1;
    int start = -1;
    int accept = -1;

    generateTM(&tm, &states, &start, &accept, argv[1]);
    
    printf("Scanning using the following matrix:\n");
    printMatrix(tm, states);

    scanInput(tm, states, start, accept);

    //free tm
    free(tm);

    //Success
    return EXIT_SUCCESS;
}
