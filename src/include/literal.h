#ifndef COMPILER_LITERAL_H
#define COMPILER_LITERAL_H

#include "variable.h"

typedef struct Literal {
	void* value;
	Type type
} Literal;

#endif COMPILER_LITERAL_H