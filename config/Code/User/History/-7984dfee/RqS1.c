#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "classes.h"
#include "matrix.h"

void createEmptyMatrix(destination **arr, int rows, int cols){
    *arr = (destination *)malloc(sizeof(destination)*rows*cols);
    
    for(int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            (*arr+(sizeof(destination)*cols*i)+(sizeof(destination)*j))->state = -1;
            (*arr+(sizeof(destination)*cols*i)+(sizeof(destination)*j))->discard = 1;
        }
    }
}

void matrixInsert(destination dest, destination *arr, int row, int col){
    (arr+(sizeof(destination)*NUM_CLASSES*row)+(sizeof(destination)*col))->state = dest.state;
    (arr+(sizeof(destination)*NUM_CLASSES*row)+(sizeof(destination)*col))->discard = dest.discard;
}

destination getMatrix(destination *arr, int row, int col){
    return *(arr+(sizeof(destination)*NUM_CLASSES*row)+(sizeof(destination)*col));
}

char* skipComments(FILE *fp, char *ptr){
    char buf[BUFLENGTH];
    
    while( (ptr = fgets(buf, BUFLENGTH, fp)) != NULL ) { 
        if(ptr[0] != '#') return ptr;
    }
    return NULL;
}

void getHeaderValue(FILE *fp, char *ptr, int *numStates, int *startState, int *acceptState){
    char *token;
    ptr = skipComments(fp, ptr);
    token = strtok(ptr, " ");
    if(strcmp(token, "states") == 0){
        token = strtok(NULL, " ");
        *numStates = (int)strtol(token, NULL, 10);
        return;
    }
    if(strcmp(token, "start") == 0){
        token = strtok(NULL, " ");
        *startState = (int)strtol(token, NULL, 10);
        return;
    }
    if(strcmp(token, "accept") == 0){
        token = strtok(NULL, " ");
        *acceptState = (int)strtol(token, NULL, 10);
        return;
    }

    return;
}

void processLine(destination *arr, char *str) {
    char *newstr = str;

    //read beginning of new line to get current state
    int currentState = strtol(str, &newstr, 10);
    char *token;

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
                                                        
        token = strtok(NULL, " ");
    }
}

void generateTM(destination **tm, int *numStates, int *startState, int *acceptState, char *fileDir){
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
    getHeaderValue(fp, ptr, numStates, startState, acceptState);
    getHeaderValue(fp, ptr, numStates, startState, acceptState);
    getHeaderValue(fp, ptr, numStates, startState, acceptState);
    
    //Create matrix and set all values to -1d
    *tm = NULL;
    createEmptyMatrix(tm, *numStates, NUM_CLASSES);
    
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
}

void printMatrix(destination *tm, int numStates){
    printf(" ");
    int i = 0;
    for(; i < NUM_CLASSES; i++){
        if(i < 10){
            printf(" ");
        }
        printf("   %d", i);
    }
    printf("\n");

    for(i = 0; i < numStates; i++){
        if(i >= 0 && i < 10){
            printf(" ");
        }
        printf("%d", i);
        for(int j = 0; j < NUM_CLASSES; j++){
            printf("  ");
            if(getMatrix(tm, i, j).state >= 0 && getMatrix(tm, i, j).state < 10){
                printf(" ");
            }
            printf("%d", getMatrix(tm, i, j).state);
            if(getMatrix(tm, i, j).discard == 1){
                printf("d");
            } else {
                printf("s");
            }
        }
        printf("\n");
    }
}
