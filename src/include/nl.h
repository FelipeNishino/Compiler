#ifndef COMPILER_NL_H
#define COMPILER_NL_H

void nl_compile(char* src);
void nl_tokenize_file(const char* filename);
void nl_compile_file(const char* filename);

#endif //COMPILER_NL_H