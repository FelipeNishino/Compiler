#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

#define FOREACH_TOKEN_TYPE(TOKEN) \
        TOKEN(identifier)   \
        TOKEN(literal)  \
        TOKEN(operat0r)   \
        TOKEN(grouper)  \
        TOKEN(UNK)  \

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,

typedef enum TokenType {
    FOREACH_TOKEN_TYPE(GENERATE_ENUM)
} TokenType;

static const char *TOKEN_TYPE_STRING[] = {
    FOREACH_TOKEN_TYPE(GENERATE_STRING)
};

#include <stdio.h>

typedef struct token {
    char* token;
    TokenType type;
} Token;

typedef struct tokenNode {
    Token token;
    struct tokenNode *next;
} TokenNode;

void printToken(Token t) { printf("(Token: {%s}, Type: {%s})\n", t.token, TOKEN_TYPE_STRING[t.type]); }

//insert link at the first location
void insertFirst(TokenNode *list, Token token);

// Check if list is empty
int isEmpty(TokenNode *list);

// return number of nodes in list
int length(TokenNode *list);

void printTokenList(TokenNode *list);

#endif //COMPILER_TOKEN_H
