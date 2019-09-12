#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#define DIE_PERROR(error_code, error, ...) do {     \
    char error_formatted[1024];                     \
    char error_formatted_line[1024];                \
                                                    \
    snprintf(error_formatted,                       \
            sizeof(error_formatted),                \
            error, ##__VA_ARGS__);                  \
    snprintf(error_formatted_line,                  \
            sizeof(error_formatted_line),           \
            "%s @ %s (%d)", error_formatted,        \
            __FILE__, __LINE__);                    \
                                                    \
    perror(error_formatted_line);                   \
    exit(error_code);                              \
} while(0)

#endif
