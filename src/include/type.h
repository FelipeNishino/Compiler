#ifndef COMPILER_TYPE_H
#define COMPILER_TYPE_H

#include "macros.h"

#define FOREACH_TYPE(TYPE) \
        TYPE(Int)   \
        TYPE(Float)   \
        TYPE(Bool)   \
        TYPE(String)   \
        TYPE(Null)   \

typedef enum Type {
    FOREACH_TYPE(GENERATE_ENUM)
} Type;
// typedef enum Type Type;

//https://stackoverflow.com/questions/8312168/static-const-char-defined-but-not-used
static const char * const TYPE_STRING[] = {
    FOREACH_TYPE(GENERATE_STRING)
};

#define FOREACH_BOOLEAN(BOOLEAN) \
        BOOLEAN(False)   \
        BOOLEAN(True)   \

typedef enum {
    FOREACH_BOOLEAN(GENERATE_ENUM)
} Boolean;
// typedef enum Type Type;

//https://stackoverflow.com/questions/8312168/static-const-char-defined-but-not-used
static const char * const BOOLEAN_STRING[] = {
    FOREACH_BOOLEAN(GENERATE_STRING)
};

typedef enum {
    I, F
} number_discriminant;

typedef struct Number {
    number_discriminant discriminant;
    union {
        int i;
        float f;
    };
} Number;

int type_compare(Type t1, Type t2);
int type_is_number(Type t);
Boolean bool_init_from_int(int r);
int number_is(Number n, number_discriminant d);
Number number_init_with_int(int i);
Number number_init_with_float(float f);

#endif //COMPILER_TYPE_H
