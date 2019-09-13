#include "file.h"

#include <stdio.h>
#include <string.h>

size_t get_delim_line(char **out, FILE *input, const size_t no_toks, const char *delim) {
    size_t len;
    *out = NULL;

    ssize_t read = getline(&(*out), &len, input);

    if (read == -1) {
        return 1;
    }

    strtok(*out, delim);

    for (size_t i = 1; i < no_toks; ++i) {
        *(out+i) = strtok(NULL, delim);
    }

    return 0;
}