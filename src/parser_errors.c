#include "include/parser_errors.h"
#include "include/token.h"
#include "include/parser.h"
#include "include/type.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

void parser_destroy_all(Parser* parser) {
    hashmap_free(parser->global_scope->var_space);
    free(parser->global_scope);
    free(parser->lexer);
    free(parser->current_token);
    free(parser->prev_token);
    free(parser);
    fprintf(stderr, "[Parser.c] Freed used resources.\n");
}

void parser_raise_error(Parser* parser, parse_error e, const char* err_str_arg, ...) {
    va_list args;
    va_start(args, err_str_arg);
    char* format = "[Parser.c:%d:%d]: %s\n";

    // format = (char*) realloc(format, strlen(format) + strlen(err_str_arg) + 8);

    // const char* template = "<type={%s}, int_type={%d}, value={%s}>";
    char* err_str;
    switch (e) {
        case illegal_assignment_constant:
            err_str = ERR_ILLEGAL_ASSIGNMENT_CONSTANT;
            break;
        case invalid_int_literal:
            err_str = ERR_INVALID_INT_LITERAL;
            break;
        case invalid_float_literal:
            err_str = ERR_INVALID_FLOAT_LITERAL;
            break;
        case invalid_operation:
            err_str = ERR_INVALID_OPERATION;
            break;
        case invalid_operation_mod:
            err_str = ERR_INVALID_OPERATION_MOD;
            break;
        case invalid_operation_pow:
            err_str = ERR_INVALID_OPERATION_POW;
            break;
        case invalid_operation_unary:
            err_str = ERR_INVALID_OPERATION_UNARY;
            break;
        case type_mismatch:
            err_str = ERR_TYPE_MISMATCH;
            break;
        case undefined_error:
            err_str = ERR_UNDEFINED_ERROR;
            err_str = malloc((strlen(err_str_arg) + 1) * sizeof(char));
            strcpy(err_str, err_str_arg);
            break;
        case undefined_identifier:
            err_str = ERR_UNDEFINED_IDENTIFIER;
            break;
        case unexpected_eof:
            err_str = ERR_UNEXPECTED_EOF;
            break;
        case unexpected_token:
            err_str = ERR_UNEXPECTED_TOKEN;
            break;
        case variable_redeclaration:
            err_str = ERR_VARIABLE_REDECLARATION;
            break;
        
    }
    // char* str = malloc((strlen(format) + strlen(err_str_arg) + 8) * sizeof(char));

    char* str = malloc((strlen(format) + strlen(err_str) + 8) * sizeof(char));
    sprintf(str, format, parser->lexer->t_pos->line, parser->lexer->t_pos->col, err_str);

    vfprintf(stderr, str, args);

    va_end(args);
    free(str);
    if (e == undefined_error) free(err_str);
    parser_destroy_all(parser);
    exit(1);
}

void parser_assert_type(Parser* parser, Type t1, Type t2) {
    if (type_compare(t1, t2)) return;
    parser_raise_error(parser, type_mismatch, NULL, TYPE_STRING[t1], TYPE_STRING[t2]);
}

void parser_assert_token_type(Parser* parser, TokenType t1, TokenType t2) {
    if (t1 == t2) return;
    parser_raise_error(parser, unexpected_token, NULL, TOKEN_STRING[t1], TOKEN_STRING[t2]);
}

void parser_assert_number_conversion(Parser* parser, str2num_errno res_code) {
    if (res_code.discriminant == DU_INT && res_code.i_errno == STR2INT_SUCCESS) return;
    if (res_code.discriminant == DU_FLOAT && res_code.f_errno == STR2FLOAT_SUCCESS) return;

    parser_raise_error(parser, (res_code.discriminant == DU_INT ? invalid_int_literal : invalid_float_literal), NULL, parser->current_token->value);
}
