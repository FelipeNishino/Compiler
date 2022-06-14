#ifndef COMPILER_VARIABLE_H
#define COMPILER_VARIABLE_H

#include "hashmap.h"
#include "macros.h"
#include "type.h"

typedef struct {
	const char* identifier;
	type type;
	void* value;
	int is_constant;
	int is_static;
} variable;

variable* variable_init();
variable* variable_init_with_id(const char* id);
void variable_print(variable var);
int variable_compare(const void *a, const void *b, void *udata);
bool variable_iter(const void *item, void *udata);
uint64_t variable_hash(const void *item, uint64_t seed0, uint64_t seed1);

#endif //COMPILER_VARIABLE_H