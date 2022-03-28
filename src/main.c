#include <stdio.h>
#include "include/iomanager.h"
#include "include/nl.h"

void printArray(char* a, int n) {
    int i = 0;
    for (i = 0; i < n; i++)
        printf(" %d ", a[i]);
}

int main(int argc, const char *argv[]) {
    char *input;
    // TokenNode *tokens;
    if (argc > 1)
        nl_compile_file(argv[1]);
    else
        input = getInputFromStdin();
    nl_compile(input);
    
    return 0;
}
