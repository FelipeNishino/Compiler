#include "include/nl.h"
#include "include/iomanager.h"
#include "include/lexer.h"
#include <stdlib.h>

void nl_compile(char* src) {
	Lexer* lexer = init_lexer(src);
	Token* token = 0;

	while ((token = lexer_read_token(lexer))->type != token_EOF)
		printf("%s\n", token_to_str(token));
	printf("%s\n", token_to_str(token));
}

void nl_compile_file(const char* filename) {
	char* src = getInputFromFile(filename) ;
	nl_compile(src);
	free(src);
}
