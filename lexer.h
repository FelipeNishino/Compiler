#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include <stdbool.h>
#include "lexer.c"

typedef struct Transition {
    int source;
    int target;
} Transition;

typedef struct State {
    int id;
    Transition* transitions;
} State;

typedef struct afd {
    State *states;
} afd;

//Transition DEFAULT_TRANSITIONS

char[] ALPHABET = "0123456789let+-*/();";
0,1,2,3,4,5,6,7,8,9,l,e,t,+,-,*,/,(,),;
void hello();

#endif //COMPILER_LEXER_H
