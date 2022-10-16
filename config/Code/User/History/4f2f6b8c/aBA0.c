#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "classes.h"
#include "matrix.h"

#define BUFLENGTH 256

char* skipComments(FILE *fp, char *ptr){
    char buf[BUFLENGTH];
    
    while( (ptr = fgets(buf, BUFLENGTH, fp)) != NULL ) { 
        if(ptr[0] != '#') return ptr;
    }
    return NULL;
}

int getHeaderValue(FILE *fp, char *ptr){
    char *token;
    ptr = skipComments(fp, ptr);
    token = strtok(ptr, " ");
    token = strtok(NULL, " ");
    return (int)strtol(token, NULL, 10);
}

void processLine(destination *arr, char *str) {
    char *newstr = str;
    //read beginning of new line to get current state
    int currentState = strtol(str, &newstr, 10);
    char *token;

    //TODO: remove
    printf("%d", currentState);
    
    token = strtok(newstr, " ");
    while(1){
        if(token == NULL) break;

        char *endptr = NULL; //create endptr
        int classIn = strtol(token, &endptr, 10);       //read beginning of token for input class
        endptr += sizeof(char);                         //move ptr 1 forward in string (skipping '/')
        int toState = strtol(endptr, &endptr, 10);       //read destination state
        int discard = (*endptr == 'd');                 //is the char after the destination state d
        destination dest = {toState, discard};          //make dest struct
        matrixInsert(dest, arr, currentState, classIn); //insert it
                                                        
        printf("  %d/%d%c", classIn, toState, *endptr); //TODO:remove

        token = strtok(NULL, " ");
    }
    printf("\n"); //TODO:reomve
}

int generateTM(destination **tm, char *fileDir){
    //Open function file
    FILE *fp;
    fp = fopen(fileDir, "r");

    //Check open was successfull
    if ( fp == NULL ) {
        //No it was not
        perror( fileDir );
        exit( EXIT_FAILURE );
    }
    
    //Read and process the 'states', start', and 'accept' lines after skipping comments
    char *ptr = NULL;
    int states = getHeaderValue(fp, ptr);
    int start = getHeaderValue(fp, ptr);
    int accept = getHeaderValue(fp, ptr);
    
    //TODO: remove
    printf("states %d\n", states);
    printf("start  %d\n", start);
    printf("accept %d\n", accept);
    
    //Create matrix and set all values to -1d
    *tm = NULL;
    createEmptyMatrix(tm, states, NUM_CLASSES);
    
    //then each line after skipping comments again
    char buf[BUFLENGTH];
    while( (ptr = fgets(buf, BUFLENGTH, fp)) != NULL ) {
        if(ptr[0] == '#'){
            continue;
        }

        //process line
        processLine(*tm, ptr);
    }
    
    //close file
    fclose( fp );

    return states;
}

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
