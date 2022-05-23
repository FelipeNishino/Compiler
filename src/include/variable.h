#ifndef COMPILER_VARIABLE_H
#define COMPILER_VARIABLE_H

#include "hashmap.h"

typedef enum Type {
	Int, Float, Bool, String, Null
} Type;

typedef struct Variable {
	const char* identifier;
	Type type;
	void* value;
	int isConstant;
	int isStatic;
} Variable;

Variable* variable_init();
Variable* variable_init_with_id(const char* id);
int variable_compare(const void *a, const void *b, void *udata);
bool variable_iter(const void *item, void *udata);
uint64_t variable_hash(const void *item, uint64_t seed0, uint64_t seed1);
int variable_assert_type(Type t1, Type t2);
// void variable_new_scope(Scope *s);

#endif //COMPILER_VARIABLE_H