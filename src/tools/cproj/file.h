#ifndef FILE_H
#define FILE_H

#include <unistd.h>
#include <stdio.h>

size_t get_delim_line(char **out, FILE *input, size_t no_toks, const char *delim);

#endif //FILE_H
