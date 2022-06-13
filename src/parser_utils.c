#include "include/parser_utils.h"
#include "include/parser_errors.h"
#include <stdlib.h>

void parser_get_token(Parser* parser) {
    parser->prev_token = parser->current_token;
    parser->current_token = lexer_read_token(parser->lexer);
}

Parser parser_make_copy(Parser* parser) {
    // Parser* p2 = calloc(1, sizeof(Parser));
    // p2->lexer =  parser->lexer;
    // p2->global_scope =  parser->global_scope;
    // p2->current_token =  parser->current_token;
    // p2->prev_token =  parser->prev_token;

    Parser* p2 = calloc(1, sizeof(Parser));
    p2->lexer = malloc(sizeof(Lexer));
    *p2->lexer = *parser->lexer;
    p2->lexer->t_pos = (text_position*) malloc(sizeof(text_position));
    p2->lexer->t_pos->line = parser->lexer->t_pos->line;
    p2->lexer->t_pos->col = parser->lexer->t_pos->col;
    p2->global_scope =  parser->global_scope;
    p2->current_token =  parser->current_token;
    p2->prev_token =  parser->prev_token;

    // return parser;

    return *p2;
}

void parser_return_to(Parser* parser, Parser p2) {
    // parser->lexer = ;
    // parser->lexer->i = i;
    // parser->lexer->c = parser->lexer->src[parser->lexer->i];


    parser->lexer->i = p2.lexer->i;
    parser->lexer->c = p2.lexer->src[p2.lexer->i];
    parser->lexer->t_pos->line = p2.lexer->t_pos->line;
    parser->lexer->t_pos->col = p2.lexer->t_pos->col;
    parser->current_token = p2.current_token;
    parser->prev_token = p2.prev_token;
}

Token* parser_find_block_end(Parser* parser, int should_return) {
    Parser pcopy = parser_make_copy(parser);
    Token* target = 0;
    int count = 1;
    do {
        parser_get_token(parser);
        switch (parser->current_token->type) {
            case token_gp_ocb:
                count++;
                break;
            case token_gp_ccb:
                count--;
                break;
            default: break;
        }
    } while(parser->current_token->type != token_EOF && count > 0);
    target = parser->current_token;
    if (should_return) parser_return_to(parser, pcopy);
    // else parser_get_token(parser);
    return target;
}


void parser_skip_block_or_statement(Parser* parser) {
    int is_block = 0;
    
    switch(lexer_peek_token(parser->lexer)->type) {
        case token_gp_ocb:
            is_block = 1;
            break;
        case token_EOF:
            parser_raise_error(parser, unexpected_eof, NULL);
        default: break;
    }
    parser_get_token(parser);
    if (is_block) parser_find_block_end(parser, 0);
    else parser_skip_to(parser, token_gp_semicolon);
}

void parser_skip_to(Parser* parser, TokenType t) {
    if (lexer_peek_token(parser->lexer)->type == token_EOF)
        parser_raise_error(parser, unexpected_eof, NULL);

    while(parser->current_token->type != t && parser->current_token->type != token_EOF) {
        parser_get_token(parser);
    }
    // parser_get_token(parser);
}

