#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "dfa.h"

TokenNode* tokenize(char* input, DFA *machine) {
	int i, j, line = 1, col = 1, tokenStart = 0,inputSize = strlen(input) + 1;
	char currentChar = 0, *auxStr = calloc(2, sizeof(char));
	Transition currentTransition;
	TokenNode *tokens = NULL;
	State currentState, state0;
    state0 = dfa_find(machine, 0)->state;
    currentState = state0;
    printf("Got state {%d}, isTerminal {%d}\n", currentState.id, currentState.isTerminal);

    printf("Initializing tokenization\n");
	for (i = 0; i < inputSize; i++) {
		currentChar = input[i];

        printf("Analyzing char {%c}\n", currentChar);        	

		if (currentChar == 10) {
            printf("Is newline, jumping to next char\n");
			line++;
			col = 1;
			continue;
		}

        printf("Searching for valid transition\n");
		for (j = 0; j < N_TRANSITIONS; j++) {
			currentTransition = DEFAULT_TRANSITIONS[j];
            printf("Testing transition {%d}; src: {%d}, chars {%s}, target {%d}\n", j, currentTransition.source, currentTransition.read, currentTransition.target);
			if (currentTransition.source == currentState.id && strchr(currentTransition.read, currentChar)) {
                printf("Is valid! Retrieving state\n");
				currentState = dfa_find(machine, currentTransition.target)->state;

//                if (currentState) {
                    printf("Got state {%d}, isTerminal {%d}\n", currentState.id, currentState.isTerminal);
//                }
//                else {
//                    printf("Couldn't find desired state on machine\n");
//                }

				if(currentState.isTerminal) {
					printf("i {%d}, tokenStart {%d}, newSize {%d}\n", i, tokenStart, (i - tokenStart + 1));
					auxStr = realloc(auxStr, i - tokenStart + 1);
					printf("Successfully reallocated auxStr");
					strncpy(auxStr, input + tokenStart, i - tokenStart);
					printf("token encontrado {%s}\n", auxStr);
					tokens = tokenList_insertFirst(tokens, (Token) {
						.token = auxStr,
						.type = currentState.id,
						.line = line,
						.col = col
						});
					currentState = state0;
					tokenStart = col;
					col++;
				}
                break;
			}
		}
		col++;
	}

	return tokens;
}