#include <stdio.h>
#include <string.h>
#include "include/iomanager.h"
#include "include/nl.h"
#include "include/variable.h"
#include "include/scope.h"

int main(int argc, const char *argv[]) {
    int lexer_only = 0;

    // Extrai as opcoes nos argumentos para o algoritmo
    if (argc < 2) fprintf(stderr, "Please provide adequate arguments.");
    for(int i = 2; i < argc; ++i) {
        // if (strcmp(argv[i], "-v") == 0) set_verbose(1);
        if (strcmp(argv[i], "-l") == 0) lexer_only = 1;
        else {
            fprintf(stderr, "[nl:main.c]: Unrecognized argument '%s'.\n", argv[i]);
            return 1;
        }
    }
    
    if (lexer_only)
        nl_tokenize_file(argv[1]);
    else 
        nl_compile_file(argv[1]);
    
    return 0;
}
