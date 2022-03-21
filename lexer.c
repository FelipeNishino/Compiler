#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "dfa.h"

TokenNode* tokenize(char* input, DFA *machine) {
	int i, j, line = 1, col = 1, inputSize = strlen(input) + 1;
	char currentChar = 0, auxStr[2] = "0";
	Transition currentTransition;
	TokenNode *tokens = NULL;
	State currentState;

	for (i = 0; i < inputSize; i++) {
		currentChar = input[i];

		if (currentChar == 10) {
			line++;
			col = 1;
			continue;
		}

		for (j = 0; j < N_TRANSITIONS; j++) {
			currentTransition = DEFAULT_TRANSITIONS[j];
			if (currentTransition.source == currentState.id && strchr(currentTransition.read, currentChar)) {
				currentState = dfa_find(machine, currentTransition.target)->state;
				
				if(currentState.isTerminal) {	
					auxStr[0] = currentChar;
					tokenList_insertFirst(tokens, (Token) {
						.token = auxStr,
						.type = currentState.id,
						.line = line,
						.col = col
						});
				}
			}
		}
		col++;
	}

	return tokens;
}