#ifndef COMPILER_LITERAL_H
#define COMPILER_LITERAL_H

#include "variable.h"

typedef struct Literal {
	void* value;
	Type type;
} Literal;

Literal* literal_init(Type type);
Literal* literal_init_with_int(int val);
Literal* literal_init_with_float(float val);
Literal* literal_init_with_string(char* val);
Literal* literal_init_with_bool(Boolean val);
Literal* literal_init_from_var(Variable* var);
Literal* literal_cast_to_float(Literal* l);
Literal* literal_cast_to_int(Literal* l);
void _literal_print_int(Literal* l);
void _literal_print_float(Literal* l);
void _literal_print_bool(Literal* l);
void _literal_print_string(Literal* l);
void literal_print(Literal* l);

#endif //COMPILER_LITERAL_H