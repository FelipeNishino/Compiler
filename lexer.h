#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include "token.h"
#include "dfa.h"

TokenNode* tokenize(char* input, DFA *machine);

#endif //COMPILER_LEXER_H
