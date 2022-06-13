#ifndef COMPILER_VARIABLE_H
#define COMPILER_VARIABLE_H

#include "hashmap.h"
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

//https://stackoverflow.com/questions/8312168/static-const-char-defined-but-not-used
static const char * const TYPE_STRING[] = {
    FOREACH_TYPE(GENERATE_STRING)
};

// typedef enum Type {
// 	Int, Float, Bool, String, Null
// } Type;

typedef struct Variable {
	const char* identifier;
	Type type;
	void* value;
	int isConstant;
	int isStatic;
} Variable;

Variable* variable_init();
Variable* variable_init_with_id(const char* id);
void variable_print(Variable var);
int variable_compare(const void *a, const void *b, void *udata);
bool variable_iter(const void *item, void *udata);
uint64_t variable_hash(const void *item, uint64_t seed0, uint64_t seed1);
int variable_compare_type(Type t1, Type t2);
int variable_is_number(Type t);
// void variable_new_scope(Scope *s);

#endif //COMPILER_VARIABLE_H