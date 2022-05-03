#include "include/parser.h"
#include "include/lexer.h"
#include "include/variable.h"
#include <stdlib.h>

Parser* init_parser(char* src) {
	Parser* parser = calloc(1, sizeof(Parser));
	parser->lexer = init_lexer(src);

	return parser;
}

void parser_compare_token_type(TokenType t1, TokenType t2) {
    if (t1 == t2) return;
    
    fprintf(stderr, "[Parser.c]: unexpected token {%s}, expected {%s}\n", TOKEN_TYPE_STRING[token->type], TOKEN_TYPE_STRING[token_grouper_op]);
    exit(1);
}

// void parser_raise_error() {

// }

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

void parser_program(Parser* parser) {
    // program ::= function
    parser_function(parser);
}

void parser_function(Parser* parser) {
    // function ::= signature block
    parser_signature(parser);
    parser_block(parser);
}

void parser_signature(Parser* parser) {
    // signature ::= identifier '(' argument* ')' ('->' type)?
    Token* token = 0;

    parser_identifier(parser);

    token = lexer_read_token(parser->lexer);
    parser_compare_token_type(token->type, token_grouper_op);

    token = lexer_read_token(parser->lexer);
    while (token->type != token_grouper_cp) {
        parser_argument(parser);
        token = lexer_read_token(parser->lexer);
    }

    parser_compare_token_type(token->type, token_grouper_cp);

    //TODO: implement return annotation
}

void parser_argument(Parser* parser) {
    // argument ::= identifier type_annotation
    parser_identifier(parser);
    parser_type_annotation(parser)
}

void parser_type_annotation(Parser* parser, Variable* var) {
    // type_annotation ::= ':' type
    Token* token = 0;

    token = lexer_read_token(parser->lexer);
    parser_compare_token_type(token->type, token_op_colon);

    var->type = parser_type(parser);
}

void parser_block(Parser* parser) {
    // block ::=  '{' (statement|control_sequence|return)+ '}'
    Token* token = 0;

    token = lexer_read_token(parser->lexer);
    parser_compare_token_type(token->type, token_grouper_ocb);

    token = lexer_read_token(parser->lexer);
    switch (token->type) {
        case token_IF:
        case token_FOR:
        case token_WHILE:
        case token_DO:
            parser_control_sequence(parser, token->type);
            break;
        case token_RETURN:
            parser_return(parser);
        default:
            parser_statement(parser);
    }

    token = lexer_read_token(parser->lexer);
    parser_compare_token_type(token->type, token_grouper_ccb)
}

void parser_control_sequence(Parser* parser, TokenType tokenType) {
    // control_sequence ::= (std_while|do_while|for|if)
    switch (tokenType) {
        case token_IF:
            parser_if(parser);
            break;
        case token_FOR:
            parser_for(parser);
            break;
        case token_WHILE:
            parser_std_while(parser);
            break;
        case token_DO:
            parser_do_while(parser);
            break;
    }
}

void parser_std_while(Parser* parser) {
    // std_while ::= 'while' condition block
}

void parser_do_while(Parser* parser) {
    // do_while ::= 'do' block 'while' condition
}

void parser_for(Parser* parser) {
    // for ::= '(' identifier assignment? ';' relational_expression ';' arithmetic_expression ')' block
}

void parser_if(Parser* parser) {
    // if ::= 'if' condition block else?
}

void parser_else(Parser* parser) {
    // else ::= 'else' block
}

void parser_condition(Parser* parser) {
    // condition ::= '(' relational_expression ')'
}

void parser_return(Parser* parser) {
    // return ::= 'return' simple_expression
}

void parser_literal(Parser* parser) {
    // literal ::= (string|number)

}

void parser_statement(Parser* parser) {
    // statement ::= (declaration|assignment)
}

void parser_declaration(Parser* parser) {
    // declaration ::= declaration_directive identifier type_annotation assignment_expression? 
}

void parser_declaration_directive(Parser* parser) {
    // declaration_directive ::= ('let'|'var')
}

void parser_assignment(Parser* parser) {
    // assignment ::= identifier assignment_expression
}

void parser_assignment_expression(Parser* parser) {
    // assignment_expression ::= '=' simple_expression
}

void parser_simple_expression(Parser* parser) {
    // simple_expression ::= (relational_expression|arithmetic_expression)
}

void parser_relational_expression(Parser* parser) {
    // relational_expression ::= unary_logical_operator? operand binary_logical_operator unary_logical_operator? operand
}

void parser_arithmetic_expression(Parser* parser) {
    // arithmetic_expression ::= grouped_arithmetic_expression|(unary_arithmetic_operator? operand binary_arithmetic_operator unary_arithmetic_operator? operand)
}

void parser_grouped_arithmetic_expression(Parser* parser) {
    // grouped_arithmetic_expression ::= '(' arithmetic_expression ')'
}

void parser_operand(Parser* parser) {
    // operand ::= (literal|identifier)
}


void parser_binary_arithmetic_operator(Parser* parser) {
    // binary_arithmetic_operator ::= higher_precedence_binary_arithmetic_operator|lesser_precedence_binary_arithmetic_operator

}

void parser_unary_arithmetic_operator(Parser* parser) {
    // unary_arithmetic_operator ::= '-'

}

void parser_unary_logical_operator(Parser* parser) {
    // unary_logical_operator ::= '!'

}


void parser_string(Parser* parser) {
    // string ::= '"' ([a-z]|[A-Z])+ '"'
}

void parser_number(Parser* parser) {
    // number ::= (integer|float)
}

void parser_integer(Parser* parser) {
    // integer ::= [0-9]+
}

void parser_float(Parser* parser) {
    // float ::= [0-9]* '.' [0-9]* ('e' (lesser_precedence_binary_arithmetic_operator)? [0-9]+)?
}

void parser_higher_precedence_binary_arithmetic_operator(Parser* parser) {
    // higher_precedence_binary_arithmetic_operator ::= ('/'|'*')
}

void parser_lesser_precedence_binary_arithmetic_operator(Parser* parser) {
    // lesser_precedence_binary_arithmetic_operator ::= ('+'|'-') 
}

void parser_binary_logical_operator(Parser* parser) {
    // binary_logical_operator ::= (higher_precedence_binary_logical_operator|lesser_precedence_binary_logical_operator)
}

void parser_higher_precedence_binary_logical_operator(Parser* parser) {
    // higher_precedence_binary_logical_operator ::= ('&&')
}

void parser_lesser_precedence_binary_logical_operator(Parser* parser) {
    // lesser_precedence_binary_logical_operator ::= ('||'|'<'|'>'|'<='|'>='|'=='|'!=')
}

void parser_identifier(Parser* parser, Variable* var) {
    // identifier ::= ([a-z]|[A-Z]) ([a-z]|[A-Z]|[0-9])*
    Token* token = lexer_read_token(parser->lexer);
    parser_compare_token_type(token->type, token_identifier);
    var->identifier = token.value;
}

Type parser_type(Parser* parser) {
    // type ::= ('Int'|'Float'|'Bool'|'String')
    switch (lexer_read_token(lexer).type) {
        case token_type_INT: return Int;
        case token_type_FLOAT: return Float;
        case token_type_BOOL: return Bool;
        case token_type_STRING: return String;
    }
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