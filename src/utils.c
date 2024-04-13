#include <stdio.h>
#include <sys/types.h>

#include "utils.h"

char *read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s\n", filename);
        return NULL;
    }
    
    char* BUFFER = NULL;
    size_t len;
    ssize_t bytes_read = getdelim(&BUFFER, &len, '\0', file);
    if (bytes_read == -1) {
        fprintf(stderr, "Failed to read file %s\n", filename);
        return NULL;
    }

    fclose(file);
    return BUFFER;
}
