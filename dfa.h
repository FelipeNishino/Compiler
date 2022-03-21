#ifndef COMPILER_DFA_H
#define COMPILER_DFA_H

static const char *STR_NUMBERS = "0123456789";
static const char *STR_LETTERS = "abcdefghijklmnopqrstuwvxyzABCDEFGHIJKLMNOPQRSTUWVXYZ";
static const char *ALPHABET = "0123456789abcdefghijklmnopqrstuwvxyzABCDEFGHIJKLMNOPQRSTUWVXYZ+-*/();";

typedef struct Transition {
    int source;
    const char *read;
    int target;
} Transition;

typedef struct State {
    int id;
    int isTerminall;
    Transition* transitions;
} State;

typedef struct afd {
    State *states;
} afd;

static const Transition DEFAULT_TRANSITIONS[] = {
    {.source = 0, .read = "+-/=", .target = 3},
    {.source = 0, .read = "()", .target = 4},
    {.source = 0, .read = "l", .target = 6},
    {.source = 0, .read = "*", .target = 9},
    {.source = 0, .read = STR_LETTERS, .target = 11},
    {.source = 0, .read = ".", .target = 12},
    {.source = 0, .read = STR_NUMBERS, .target = 13},
    {.source = 6, .read = "e", .target = 7},
    {.source = 7, .read = "t", .target = 8},
    {.source = 8, .read = " ", .target = 3},
    {.source = 9, .read = " ", .target = 3},
    {.source = 9, .read = "*", .target = 10},
    {.source = 10, .read = " ", .target = 3},
    {.source = 11, .read = " ", .target = 1},
    {.source = 11, .read = "abcdefghijklmnopqrstuwvxyzABCDEFGHIJKLMNOPQRSTUWVXYZ0123456789", .target = 11},
    {.source = 12, .read = STR_NUMBERS, .target = 13},
    {.source = 13, .read = " ", .target = 2},
    {.source = 13, .read = ".", .target = 12},
    {.source = 13, .read = STR_NUMBERS, .target = 13},
    {.source = 13, .read = "eE", .target = 14},
    {.source = 14, .read = "-", .target = 15},
    {.source = 14, .read = STR_NUMBERS, .target = 16},
    {.source = 15, .read = STR_NUMBERS, .target = 16},
    {.source = 16, .read = " ", .target = 2},    
    {.source = 16, .read = STR_NUMBERS, .target = 16},
};

#endif //COMPILER_DFA_H
