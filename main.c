//
// Created by Nishi on 14/03/2022.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

#define BUFFER_SIZE 256
#define STDIN_BUFFER_SIZE 1024

char *getInputFromFile(const char *filename){ 
    FILE *fp;
    char basename[20] = "./";
    char lineBuffer[256];
    int currentBufferSize = 0;
    int currentBytes
    char *input = calloc(256, sizeof(char));

    // Abre o arquivo com o caminho ".\filename"
    strcat(basename, filename);
    fp = fopen(basename, "r");
    // Checa se o fopen foi bem sucedido
    if (!fp)  {
        printf("Couldn't find file named '%s', make sure to make the extension explicit.", filename);
        fclose(fp);
        return NULL;
    }
    
    while (fgets(lineBuffer, 256, fp)) {
        currentBufferSize += 256;
        if (realloc(input, currentBufferSize * sizeof(char)) == NULL) {
            printf("Error during memory reallocation.\n");
            return NULL;
        }

    }

    return input;
}

void printArray(char* a, int n) {
    int i = 0;
    for (i = 0; i < n; i++)
        printf(" %d ", a[i]);
}

char *getInputFromStdin() {
    int line, charPos, nChars = 0, bufferLimit = 1024;
    char currentChar;
    char auxStr[2] = {0};
    char *buffer = calloc(STDIN_BUFFER_SIZE, sizeof(char));

    line = 0;
    charPos = 0;

    while (1) {
        currentChar = getchar();
        nChars++;

        if (nChars > 1 && currentChar == 10 && buffer[nChars-2] == 10) {
            break;
        }
        if (nChars == bufferLimit) {
            bufferLimit += 1024;
            if (realloc(buffer, bufferLimit * sizeof(char)) == NULL) {
                printf("Error during memory reallocation.\n");
                return NULL;
            }
        }
        
        auxStr[0] = currentChar;
        strcat(buffer, auxStr);
    }
    return buffer;
}

int main(int argc, const char *argv[]) {
    char *input = getInputFromStdin();
    // printf("Input recebido: /{/%s/}/", input);

    return 0;
}
