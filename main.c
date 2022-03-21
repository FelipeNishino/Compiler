#include <stdio.h>
#include "iomanager.h"
#include "lexer.h"
#include "token.h"
#include "dfa.h"

void printArray(char* a, int n) {
    int i = 0;
    for (i = 0; i < n; i++)
        printf(" %d ", a[i]);
}

int main(int argc, const char *argv[]) {
    char *input;
    TokenNode *tokens;
    if (argc > 1) {
        input = getInputFromFile(argv[1]);
    }
    else
        input = getInputFromStdin();   

    DFA *machine = dfa_createMachine();

//    dfa_printList(machine);

    tokens = tokenize(input, machine);
    tokenList_printList(tokens);
    return 0;
}
