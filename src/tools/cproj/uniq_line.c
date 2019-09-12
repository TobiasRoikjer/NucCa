#include <string.h>

#include "utils.h"

int main(int argc, char **argv) {
    fprintf(stderr, "Start unique lines\n");

    if (argc != 2) {
        fprintf(stderr, "Usage: <file>\n");
        exit(1);
    }

    size_t i = 0;
    char *strings[] = { NULL, NULL };
    char **line = strings;
    char **prev_line = (strings + 1);
    size_t len;
    ssize_t read;

    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        DIE_PERROR(1, "Failed to open input file %s\n", argv[1]);
    }

    while ((read = getline(line, &len, file)) != -1) {
        if ((*line)[read - 1] == '\n') {
            (*line)[read - 1] = '\0';
        }

        if (*prev_line == NULL || strcmp(*line, *prev_line) != 0) {
            if (*prev_line != NULL ) {
                fprintf(stdout, "\n");
            }

            fprintf(stdout, "%s", *line);
        }

        i = (i + 1);
        i = i % 2;

        line = (strings + i);
        prev_line = (strings + (1 - i));
    }

    free(*line);
    free(*prev_line);

    fclose(file);

    fprintf(stderr, "End unique lines\n");
}