#include "include/parser.h"
#include "include/lexer.h"
#include "include/variable.h"
#include <stdlib.h>

Parser* init_parser(char* src) {
	Parser* parser = calloc(1, sizeof(Parser));
	parser->lexer = init_lexer(src);

	return parser;
}

int parser_parse(Parser* parser) {
	// Token* token = 0;

	// do {
	// 	token = lexer_read_token(parser->lexer);
	// }
	// while (token->type != token_EOF);
	
	parser_program(parser);

	// printf("%s\n", token_to_str(token));
	// printf("printou todos os simbolos");
	return 0;
}

int parser_program(Parser* parser) {
	return 0;
}

/*
int cfr_generate_frequency_chart(struct hashmap *map, char* src) {
    int i = 0;
    int *cf_array = calloc(95, sizeof(int));
    // total 95
    // comeco 32
    while(src[i]) {
        if (src[i] == 10) continue;
        // printf("Vai tentar incrementar a freq em %d para o char %d\n", src[i]-32, src[i]);
        cf_array[src[i]-32]++;
        i++;
    }

    for (i = 0; i < 95; ++i)
    {
        printf("char (%d,%c), f %d\n", i+32, i+32, cf_array[i]);
    }

    

    // hashmap_set(map, &(struct user){ .name="Dale", .age=44 });
    //     // user = hashmap_get(map, &(struct user){ .name="Jane" });

    // printf("\n-- iterate over all users (hashmap_scan) --\n");
    // hashmap_scan(map, cfr_iter, NULL);

    // printf("\n-- iterate over all users (hashmap_iter) --\n");
    // size_t iter = 0;
    // void *item;
    // while (hashmap_iter(map, &iter, &item)) {
    //     const struct user *user = item;
    //     printf("%s (age=%d)\n", user->name, user->age);
    // }
    // hashmap_free(map);
    return 0;
}*/