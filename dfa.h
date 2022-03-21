#ifndef COMPILER_DFA_H
#define COMPILER_DFA_H

#define STR_NUMBERS "0123456789"
#define STR_LETTERS "abcdefghijklmnopqrstuwvxyzABCDEFGHIJKLMNOPQRSTUWVXYZ"
#define ALPHABET "0123456789abcdefghijklmnopqrstuwvxyzABCDEFGHIJKLMNOPQRSTUWVXYZ+-*/();"

#include <stdlib.h>

typedef struct Transition {
    int source;
    const char *read;
    int target;
} Transition;

typedef struct transitionNode {
    Transition transition;
    struct transitionNode *next;
} TransitionNode;

typedef struct state {
    int id;
    int isTerminal;
} State;

typedef struct dfa {
    State state;
    struct dfa *next;
} DFA;

static const int N_TRANSITIONS = 25;
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

// State MACHINE_STATES[] = {
    // {.id = 0, .isTerminal = 0, .transitions = {
    //     (Transition) {.source = 0, .read = "+-/=", .target = 3},
    //     (Transition) {.source = 0, .read = "()", .target = 4},
    //     (Transition) {.source = 0, .read = "l", .target = 6},
    //     (Transition) {.source = 0, .read = "*", .target = 9},
    //     (Transition) {.source = 0, .read = STR_LETTERS, .target = 11},
    //     (Transition) {.source = 0, .read = ".", .target = 12},
    //     (Transition) {.source = 0, .read = STR_NUMBERS, .target = 13}
    // }},
    // {.id = 1, .isTerminal = 1, .transitions = {}},
    // {.id = 2, .isTerminal = 1, .transitions = {}},
    // {.id = 3, .isTerminal = 1, .transitions = {}},
    // {.id = 4, .isTerminal = 1, .transitions = {}},
    // {.id = 5, .isTerminal = 1, .transitions = {}},
    // {.id = 6, .isTerminal = 0, .transitions = {{.source = 6, .read = "e", .target = 7}}},
    // {.id = 7, .isTerminal = 0, .transitions = {{.source = 7, .read = "t", .target = 8}}},
    // {.id = 8, .isTerminal = 0, .transitions = {{.source = 8, .read = " ", .target = 3}}},
    // {.id = 9, .isTerminal = 0, .transitions = {
    //     {.source = 9, .read = " ", .target = 3},
    //     {.source = 9, .read = "*", .target = 10}
    // }},
    // {.id = 10, .isTerminal = 0, .transitions = {{.source = 10, .read = " ", .target = 3},}},
    // {.id = 11, .isTerminal = 0, .transitions = {
    //     {.source = 11, .read = " ", .target = 1},
    //     {.source = 11, .read = "abcdefghijklmnopqrstuwvxyzABCDEFGHIJKLMNOPQRSTUWVXYZ0123456789", .target = 11},
    // }},
    // {.id = 12, .isTerminal = 0, .transitions = {{.source = 12, .read = STR_NUMBERS, .target = 13}}},
    // {.id = 13, .isTerminal = 0, .transitions = {
    //     {.source = 13, .read = " ", .target = 2},
    //     {.source = 13, .read = ".", .target = 12},
    //     {.source = 13, .read = STR_NUMBERS, .target = 13},
    //     {.source = 13, .read = "eE", .target = 14}
    // }},
    // {.id = 14, .isTerminal = 0, .transitions = {
    //     {.source = 14, .read = "-", .target = 15},
    //     {.source = 14, .read = STR_NUMBERS, .target = 16}
    // }},
    // {.id = 15, .isTerminal = 0, .transitions = {{.source = 15, .read = STR_NUMBERS, .target = 16}}},
    // {.id = 16, .isTerminal = 0, .transitions = {
    // {.source = 16, .read = " ", .target = 2},    
    // {.source = 16, .read = STR_NUMBERS, .target = 16}
    // }}
// };

//insert link at the first location
void transition_insert(TransitionNode *list, Transition transition);
TransitionNode* transition_find(TransitionNode *list, Transition transition);
int transition_isEmpty(TransitionNode *list);
int transition_isEqual(Transition t1, Transition t2);
void dfa_insertState(DFA *dfa, State state);
//is list empty
int dfa_isEmpty(DFA *dfa);
//find a link with given key
DFA* dfa_find(DFA *dfa, int id);
int dfa_length(DFA *dfa);
DFA *dfa_createMachine();
#endif //COMPILER_DFA_H
