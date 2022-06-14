#include "include/parser_utils.h"
#include "include/parser_errors.h"
#include <stdlib.h>

void parser_get_token(parser* p) {
    p->prev_token = p->current_token;
    p->current_token = lexer_read_token(p->lexer);
}

parser parser_make_copy(parser* p) {
    // parser* p2 = calloc(1, sizeof(parser));
    // p2->lexer =  p->lexer;
    // p2->global_scope =  p->global_scope;
    // p2->current_token =  p->current_token;
    // p2->prev_token =  p->prev_token;

    parser* p2 = calloc(1, sizeof(parser));
    p2->lexer = malloc(sizeof(lexer));
    *p2->lexer = *p->lexer;
    p2->lexer->t_pos = (text_position*) malloc(sizeof(text_position));
    p2->lexer->t_pos->line = p->lexer->t_pos->line;
    p2->lexer->t_pos->col = p->lexer->t_pos->col;
    p2->global_scope =  p->global_scope;
    p2->current_token =  p->current_token;
    p2->prev_token =  p->prev_token;

    // return p;

    return *p2;
}

void parser_return_to(parser* p, parser p2) {
    // p->lexer = ;
    // p->lexer->i = i;
    // p->lexer->c = p->lexer->src[p->lexer->i];


    p->lexer->i = p2.lexer->i;
    p->lexer->c = p2.lexer->src[p2.lexer->i];
    p->lexer->t_pos->line = p2.lexer->t_pos->line;
    p->lexer->t_pos->col = p2.lexer->t_pos->col;
    p->current_token = p2.current_token;
    p->prev_token = p2.prev_token;
}

token* parser_find_block_end(parser* p, int should_return) {
    parser pcopy = parser_make_copy(p);
    token* target = 0;
    int count = 1;
    do {
        parser_get_token(p);
        switch (p->current_token->type) {
            case token_gp_ocb:
                count++;
                break;
            case token_gp_ccb:
                count--;
                break;
            default: break;
        }
    } while(p->current_token->type != token_EOF && count > 0);
    target = p->current_token;
    if (should_return) parser_return_to(p, pcopy);
    // else parser_get_token(p);
    return target;
}


void parser_skip_block_or_statement(parser* p) {
    int is_block = 0;
    
    switch(lexer_peek_token(p->lexer)->type) {
        case token_gp_ocb:
            is_block = 1;
            break;
        case token_EOF:
            parser_raise_error(p, unexpected_eof, NULL);
        default: break;
    }
    parser_get_token(p);
    if (is_block) parser_find_block_end(p, 0);
    else parser_skip_to(p, token_gp_semicolon);
}

void parser_skip_to(parser* p, token_type t) {
    if (lexer_peek_token(p->lexer)->type == token_EOF)
        parser_raise_error(p, unexpected_eof, NULL);

    while(p->current_token->type != t && p->current_token->type != token_EOF) {
        parser_get_token(p);
    }
    // parser_get_token(p);
}

