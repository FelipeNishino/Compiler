#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "include/iomanager.h"

char *iomanager_get_input_from_file(const char *filename){ 
    FILE *fp;
    char basename[20] = "./";
    char line_buffer[256];
    int current_buffer_size = 1;
    char *input = calloc(1, sizeof(char));

    // Abre o arquivo com o caminho ".\filename"
    strcat(basename, filename);
    fp = fopen(basename, "r");
    // Checa se o fopen foi bem sucedido
    if (!fp)  {
        printf("Couldn't find file named '%s', make sure to make the extension explicit.", filename);
        exit(1);
    }
    
    // printf("Opened input file successfully, reading contents...\n");

    while (fgets(line_buffer, 256, fp)) {
        current_buffer_size += strlen(line_buffer);

        input = realloc(input, current_buffer_size * sizeof(char));
        if (input == NULL) {
            printf("Error during memory reallocation.\n");
            return NULL;
        }

        strcat(input, line_buffer);
    }

    fclose(fp);

    return input;
}

char *iomanager_get_input_from_stdin() {
    int n_chars = 0, buffer_limit = 1024;
    char current_char;
    char aux_str[2] = {0};
    char *buffer = calloc(STDIN_BUFFER_SIZE, sizeof(char));
    
    while (1) {
        current_char = getchar();
        n_chars++;

        if (n_chars > 1 && current_char == 10 && buffer[n_chars-2] == 10) {
            break;
        }
        if (n_chars == buffer_limit) {
            buffer_limit += 1024;
            if (realloc(buffer, buffer_limit * sizeof(char)) == NULL) {
                printf("Error during memory reallocation.\n");
                return NULL;
            }
        }
        
        aux_str[0] = current_char;
        strcat(buffer, aux_str);
    }
    return buffer;
}
