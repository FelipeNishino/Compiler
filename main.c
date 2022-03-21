#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "lexer.h"
#include "iomanager.h"

void printArray(char* a, int n) {
    int i = 0;
    for (i = 0; i < n; i++)
        printf(" %d ", a[i]);
}

int main(int argc, const char *argv[]) {
    char *input;
    if (argc > 1) {
        input = getInputFromFile(argv[1]);
    }
    else
        input = getInputFromStdin();   

    // char *input = getInputFromStdin();
    
    printf("Input recebido: /{/%s/}/", input);

    return 0;
}
