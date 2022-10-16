#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "classes.h"
#include "scanner.h"
#include "matrix.h"

void scanInput(destination *tm, int state, int start, int accept){
    int classRead = -1;
    int currentState = start;
    char token[TOKENLEN];
    int cursor = 0;
    int c;
    printf("%d", start);
    while(1){
        c = getchar();
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
            if(classRead != CC_WS && classRead != CC_NL && classRead != CC_EOF){
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
}