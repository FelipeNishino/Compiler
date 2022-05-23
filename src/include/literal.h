#ifndef COMPILER_LITERAL_H
#define COMPILER_LITERAL_H

#include "variable.h"

typedef struct Literal {
	void* value;
	Type type;
} Literal;

Literal literal_init(void* value, Type type);

#endif //COMPILER_LITERAL_H