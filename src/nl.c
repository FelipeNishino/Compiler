#include "include/nl.h"
#include "include/iomanager.h"
#include "include/parser.h"
#include <stdlib.h>

void nl_compile(char* src) {
	Parser* parser = init_parser(src);

	if (parser_parse(parser)) {
		fprintf(stderr, "[Parser.c]: Failed parsing source code\n");
		exit(1);
	}
}

void nl_compile_file(const char* filename) {
	char* src = getInputFromFile(filename) ;
	nl_compile(src);
	free(src);
}
