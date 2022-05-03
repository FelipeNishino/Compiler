#ifndef COMPILER_SCOPE_H
#define COMPILER_SCOPE_H

#include "hashmap.h"
#include "variable.h"

typedef struct Scope {
	struct hashmap varSpace;

} Scope;

Scope* scope_init();
void scope_set_variable(Scope* scope, Variable var);
void scope_get_variable(Scope* scope, Variable var);
void scope_get_variable_by_id(Scope* scope, const char* identifier);

#endif COMPILER_SCOPE_H