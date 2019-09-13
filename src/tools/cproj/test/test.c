#include <libgen.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "../file.h"
#include "../utils.h"

void get_delim_line_reads_first_line() {
    FILE *file = fopen("./test/data/sample.csv", "r");

    if (file == NULL)
    {
        DIE_PERROR(1, "Failed to open sample.csv file\n");
    }

    char *chars[3];

    get_delim_line(chars, file, 3, ",");

    fclose(file);

    assert(strcmp(chars[0], "test") == 0);
    assert(strcmp(chars[1], "hest") == 0);
    assert(strncmp(chars[2], "fest", 4) == 0);
    free(chars[0]);
}

void get_delim_line_reads_limited_tokens() {
    FILE *file = fopen("./test/data/sample.csv", "r");

    if (file == NULL)
    {
        DIE_PERROR(1, "Failed to open sample.csv file\n");
    }

    char *chars[1];

    get_delim_line(chars, file, 1, ",");
    fclose(file);

    assert(strcmp(chars[0], "test") == 0);
    free(chars[0]);
}


void get_delim_line_reads_multiple_lines() {
    FILE *file = fopen("./test/data/sample.csv", "r");

    if (file == NULL)
    {
        DIE_PERROR(1, "Failed to open sample.csv file\n");
    }

    char *chars[3];

    get_delim_line(chars, file, 2, ",");
    assert(strcmp(chars[0], "test") == 0);

    get_delim_line(chars, file, 2, ",");
    assert(strcmp(chars[0], "foo") == 0);

    get_delim_line(chars, file, 2, ",");
    assert(strcmp(chars[0], "man") == 0);

    get_delim_line(chars, file, 3, ",");
    assert(strcmp(chars[0], "0") == 0);
    assert(strcmp(chars[1], "0") == 0);
    assert(strncmp(chars[2], "0", 1) == 0);

    get_delim_line(chars, file, 2, ",");
    assert(strcmp(chars[0], "1") == 0);

    assert(get_delim_line(chars, file, 2, ",") == 1);
    free(chars[0]);
    fclose(file);
}

int main(int argc, char **argv) {
    get_delim_line_reads_first_line();
    get_delim_line_reads_limited_tokens();
    get_delim_line_reads_multiple_lines();

    return 0;
}
