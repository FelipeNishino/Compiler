#ifndef COMPILER_IOMANAGER_H
#define COMPILER_IOMANAGER_H

#define BUFFER_SIZE 256
#define STDIN_BUFFER_SIZE 1024

char *iomanager_get_input_from_file(const char *filename);
char *iomanager_get_input_from_stdin();

#endif //COMPILER_IOMANAGER_H
