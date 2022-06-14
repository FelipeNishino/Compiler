#ifndef COMPILER_TYPE_H
#define COMPILER_TYPE_H

#include "macros.h"

#define FOREACH_TYPE(TYPE) \
        TYPE(Int)   \
        TYPE(Float)   \
        TYPE(Bool)   \
        TYPE(String)   \
        TYPE(Null)   \

typedef enum {
    FOREACH_TYPE(GENERATE_ENUM)
} type;

//https://stackoverflow.com/questions/8312168/static-const-char-defined-but-not-used
static const char * const TYPE_STRING[] = {
    FOREACH_TYPE(GENERATE_STRING)
};

#define FOREACH_BOOLEAN(BOOLEAN) \
        BOOLEAN(False)   \
        BOOLEAN(True)   \

typedef enum {
    FOREACH_BOOLEAN(GENERATE_ENUM)
} boolean;

//https://stackoverflow.com/questions/8312168/static-const-char-defined-but-not-used
static const char * const BOOLEAN_STRING[] = {
    FOREACH_BOOLEAN(GENERATE_STRING)
};

typedef enum {
    I, F
} number_discriminant;

typedef struct {
    number_discriminant discriminant;
    union {
        int i;
        float f;
    };
} number;

int type_compare(type t1, type t2);
int type_is_number(type t);
boolean bool_init_from_int(int r);
int number_is(number n, number_discriminant d);
number number_init_with_int(int i);
number number_init_with_float(float f);

#endif //COMPILER_TYPE_H
