#ifndef COMPILER_MACROS_H
#define COMPILER_MACROS_H
#define LEN(x) (sizeof(x) / sizeof((x)[0]))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#endif// COMPILER_MACROS_H