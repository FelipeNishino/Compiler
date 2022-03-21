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
    currentState = dfa_find(machine, 0)->state;
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
					auxStr[0] = currentChar;
					tokenList_insertFirst(tokens, (Token) {
						.token = auxStr,
						.type = currentState.id,
						.line = line,
						.col = col
						});
				}
                break;
			}
		}
		col++;
	}

	return tokens;
}