#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "classes.h"
#include "matrix.h"

int charToClass(int c){
    //0 : CC_WS
    if(c == ' ' || c == '\t') return CC_WS;

    //1 : CC_NL
    if(c == '\n') return CC_NL;

    //2 : CC_ALPHA
    if(isalpha(c) || c == '_') return CC_ALPHA;

    //3 : CC_ZERO
    if(c == '0') return CC_ZERO;

    //4 : CC_OCTAL
    if(c >= '1' && c <= '7') return CC_OCTAL;

    //5 : CC_DECIMAL
    if(c == '8' || c == '9') return CC_DECIMAL;

    //6 : CC_SLASH
    if(c == '/') return CC_SLASH;

    //7 : CC_STAR
    if(c == '*') return CC_STAR;

    //8 ; CC_ARITH
    if(c == '+' || c == '-' || c == '%') return CC_ARITH;

    //9 : CC_OTHER
    if( c >= 0 ) return CC_OTHER;

    //10 : CC_EOF
    if(c == EOF) return CC_EOF;

    //11 : CC_ERROR
    return CC_ERROR;
}

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

    int classRead = -1;
    int currentState = start;
    char token[BUFLENGTH];
    int cursor = 0;
    int c;
    printf("%d", start);
    while(1){
        c = getchar();
        printf("%c", c);
        continue;
        classRead = charToClass(c);

        //if accepted
        if(currentState == accept){
            token[cursor] = '\0';
            cursor = 0;
            printf(" recognized '%s'\n", token);
            currentState = start;
            printf("%d", start);
        }

        //If rejected
        if(currentState == -1){
            //skip until whitespace
            if(classRead != CC_WS && classRead != CC_EOF){
                continue;
            }

            //then print
            token[cursor] = '\0';
            cursor = 0;
            printf(" rejected\n");
            currentState = start;
            printf("%d", start);
            continue;
        }

        //If EOF
        if(classRead == CC_EOF){
            currentState = getMatrix(tm, currentState, classRead).state;
            printf(" %d EOF\n", currentState);
            break;
        }

        if(classRead == CC_ERROR){
            perror("Stdin: invalid input\n");
            break;
        }

        if( getMatrix(tm, currentState, classRead).discard == 0 ){
            token[cursor] = c;
            cursor++;
        }

        currentState = getMatrix(tm, currentState, classRead).state;
        printf(" %d", currentState);
    }

    //free tm
    free(tm);

    //Success
    return EXIT_SUCCESS;
}
