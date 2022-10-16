#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "classes.h"
#include "matrix.h"


int main(int argc, char* argv[]){
    //assert argc == 2
    if ( argc != 2 ) {
        fprintf(stderr, "usage: %s tmfile\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    destination *tm;
    int numStates = generateTM(&tm, argv[1]);

    //free tm
    free(tm);
    
    //Success
    return EXIT_SUCCESS;
}
