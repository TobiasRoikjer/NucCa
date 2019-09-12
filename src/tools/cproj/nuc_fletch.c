#include <stdio.h>
#include <libgen.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>

#include "utils.h"
#include "ds.h"

const char *CHROMS[] = { "chr1", "chr2", "chr3", "chr4", "chr5", "chr6", "chr7", "chr8", "chr9", "chr10", "chr11", "chr12", "chr13", "chr14", "chr15", "chr16", "chr17", "chr18", "chr19", "chr20", "chr21", "chrX", "chrY" };

typedef struct nuc_file_data {
    const char *chrom;
    long start;
    long stop;
} nuc_file_data;

static inline void get_next_nuc_line(FILE *file, const char *chrom_to, nuc_file_data *data) {
    char *line = NULL;
    size_t len;

    ssize_t read;
    char *chrom = NULL, *start = NULL, *stop = NULL;

    while (true) {
        if ((read = getdelim(&chrom, &len, (int) '\t', file)) != -1) {
            chrom[read - 1] = '\0';
        } else {
            break;
        }

        if ((read = getdelim(&start, &len, (int) '\t', file)) != -1) {
            start[read - 1] = '\0';
        } else {
            break;
        }

        if ((read = getdelim(&stop, &len, (int) '\t', file)) != -1) {
            stop[read - 1] = '\0';
        } else {
            break;
        }

        if ((getline(&line, &len, file)) != -1) {
        } else {
            break;
        }

        if (strcmp(chrom, chrom_to) == 0) {
            free((void*)data->chrom);
            data->chrom = chrom;
            data->start = atol(start);
            data->stop = atol(stop);
            free(start);
            free(stop);
            free(line);
            return;
        }
    }

    free(chrom);
    free(start);
    free(stop);
    free(line);
    data->chrom = NULL;
}

/**
 * Returns a list of files to parse, relative to {@param filename} of size
 *  given by {@param n_out}.
 *
 * @param out
 * @param n_out
 * @param filename
 */
void parse_nuc_file_list(const char ***out, size_t *n_out, const char *filename)
{
    char **line;
    size_t len;
    ssize_t read;

    ll *list = NULL;
    size_t lines = 0;
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        DIE_PERROR(1, "Failed to open nuc list file %s\n", filename);
    }

    line = malloc(sizeof(char*));

    while ((read = getline(line, &len, file)) != -1) {
        if ((*line)[read - 1] == '\n') {
            (*line)[read - 1] = '\0';
        }

        lines++;
        list = ll_add(list, *line);
        line = malloc(sizeof(char*));
    }

    fclose(file);

    *out = malloc(sizeof(char*) * lines);
    *n_out = lines;
    ll_to_array((void**)*out, list);
    ll_free(list);
}

void write_nuc_lines(FILE *out, const char **filenames, size_t n_filenames, const char *chrom_to) {
    FILE *files[n_filenames];

    nuc_file_data res[n_filenames];

    for (size_t i = 0; i < n_filenames; ++i) {
        files[i] = fopen(filenames[i], "r");

        if (files[i] == NULL)
        {
            DIE_PERROR(1, "Failed to open nuc file %s\n", filenames[i]);
        }

        res[i].chrom = NULL;

        get_next_nuc_line(files[i], chrom_to, res+i);
    }

    long min_start;
    size_t min_index = 0;
    char buffer[1024];

    do {
        min_start = LONG_MAX;

        for (size_t i = 0; i < n_filenames; ++i) {
            if (res[i].chrom != NULL) {
                if (res[i].start < min_start) {
                    min_start = res[i].start;
                    min_index = i;
                }
            }
        }

        if (min_start != LONG_MAX) {
            sprintf(buffer, "%s\t%ld\t%ld\n",
                    res[min_index].chrom,
                    res[min_index].start,
                    res[min_index].stop);
            fwrite(buffer, strlen(buffer), 1, out);

            get_next_nuc_line(files[min_index],
                            chrom_to, res + min_index);
        }
    } while (min_start != LONG_MAX);

    for (size_t i = 0; i < n_filenames; ++i) {
        fclose(files[i]);
    }
}

int main (int argc, char **argv) {
    fprintf(stderr, "Start nucleosome fletching\n");

    if (argc != 2) {
        fprintf(stderr, "Usage: <nuc list>\n");
        exit(1);
    }

    const char **out;
    size_t n;
    parse_nuc_file_list(&out, &n, argv[1]);


    fprintf(stderr, "No. of files: %zu\n", n);

    fprintf(stderr, "Chromosomes used: ");

    for (size_t i = 0; i < sizeof(CHROMS)/sizeof(CHROMS[0]); ++i) {
        fprintf(stderr, "%s, ", CHROMS[i]);
    }

    fprintf(stderr, "\n");

    //FILE *f = fopen("t.out", "w");

    for (size_t i = 0; i < sizeof(CHROMS)/sizeof(CHROMS[0]); ++i) {
        fprintf(stderr, "Parsing chromosome: %s...\n", CHROMS[i]);
        write_nuc_lines(stdout, out, n, CHROMS[i]);
    }

    //fclose(f);

    fprintf(stderr, "End nucleosome fletching\n");

    return 0;
}