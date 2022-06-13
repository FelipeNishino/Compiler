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
Literal* literal_init_with_bool(int val);
Literal* literal_init_from_var(Variable* var);
void literal_print(Literal* l);
void _literal_print_int(Literal* l);
void _literal_print_float(Literal* l);
void _literal_print_bool(Literal* l);
void _literal_print_string(Literal* l);

#endif //COMPILER_LITERAL_H