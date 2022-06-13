#ifndef COMPILER_VARIABLE_H
#define COMPILER_VARIABLE_H

#include "hashmap.h"
#include "macros.h"
#include "type.h"

struct Variable {
	const char* identifier;
	Type type;
	void* value;
	int isConstant;
	int isStatic;
};

typedef struct Variable Variable;

Variable* variable_init();
Variable* variable_init_with_id(const char* id);
void variable_print(Variable var);
int variable_compare(const void *a, const void *b, void *udata);
bool variable_iter(const void *item, void *udata);
uint64_t variable_hash(const void *item, uint64_t seed0, uint64_t seed1);

#endif //COMPILER_VARIABLE_H