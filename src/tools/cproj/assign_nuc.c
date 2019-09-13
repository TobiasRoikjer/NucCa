#define _GNU_SOURCE

#include <string.h>
#include <stdint.h>
#include <math.h>

#include "utils.h"
#include "file.h"

#define MAX_LENGTH 300
#define MAX_DIST 1000
#define COL_CHR 0
#define COL_POS1 1
#define COL_POS2 2
#define COL_MID 3

unsigned long data[MAX_DIST][MAX_LENGTH] = {0};

void retrieve_data(FILE *reads, FILE *nucs, char *delimiter) {
    /* Algorithm:
     * Keep two pointers:
     *  - One at the nucleosome file (NF) at the "current" line and the
     *      previous line
     *  - One at the reads file (RF)
     * Move the NF pointer down if:
     *      a) the chromosome of RF is greater
     *      b) the midpoint of RF is greater than the midpoint of the
     *          current line, and of the same chromosome
     * Move the RF pointer down if:
     *      a) the chromosome of NF is greater
     *
     * These steps gives us two midpoints in the nucleosome file, which form an
     * interval around a read, i.e. one is smaller and one is larger, or in a
     * special case (e.g. beginning/end of chromosome) only one position, which
     * is either smaller or greater.
     *
     * It also skips reads where we do not have a single nucleosome for a
     * particular chromosome.
     *
     * TODO: Perhaps reduce the number of string comparisons?
     */

    char *nuc_chrom_high = NULL;
    char *nuc_chrom_low = NULL;
    char *read_chrom = NULL;

    long read_midpoint = -1;
    long nuc_midpoint_low = -1;
    long nuc_midpoint_high = -1;

    char *fragment[4];
    char *nucleosome[2][4];
    int i = 0;

    if (get_delim_line(nucleosome[i], nucs, 4, delimiter)) {
        fprintf(stderr, "Warning: Failed to read first line of nucleosome file");
        return;
    }

    nuc_midpoint_low = atol(nucleosome[0][COL_MID]);
    nuc_chrom_low = nucleosome[0][COL_CHR];

    nuc_midpoint_high = atol(nucleosome[0][COL_MID]);
    nuc_chrom_high = nucleosome[0][COL_CHR];

    while (get_delim_line(fragment, reads, 4, delimiter) == 0) {
        read_midpoint = atol(fragment[COL_MID]);
        read_chrom = fragment[COL_CHR];

        if (strverscmp(read_chrom, nuc_chrom_low) < 0) {
            continue;
        }

        while ((nuc_midpoint_high < read_midpoint
                && (nuc_chrom_high == NULL || strverscmp(read_chrom, nuc_chrom_high) >= 0))
                || (nuc_chrom_low == NULL || strverscmp(read_chrom, nuc_chrom_low) > 0)) {
            nuc_midpoint_low = nuc_midpoint_high;
            nuc_chrom_low = nuc_chrom_high;

            i++;
            i = i % 2;

            if (get_delim_line(nucleosome[i], nucs, 4, delimiter) != 0) {
                break;
            }

            nuc_midpoint_high = atol(nucleosome[i][COL_MID]);
            nuc_chrom_high = nucleosome[i][COL_CHR];
        }

        long dist = -1;

        if (nuc_chrom_low != NULL && strcmp(read_chrom, nuc_chrom_low) == 0) {
            dist = labs(read_midpoint - nuc_midpoint_low);
        }

        if (nuc_chrom_high != NULL && strcmp(read_chrom, nuc_chrom_high) == 0) {
            long high_dist = labs(read_midpoint - nuc_midpoint_high);

            dist = dist == -1 || high_dist < dist ? high_dist : dist;
        }

        if (dist != -1) {
            long length = labs(atol(fragment[COL_POS1]) - atol(fragment[COL_POS2]));

            if (dist < MAX_DIST && length < MAX_LENGTH) {
                data[dist][length]++;
            }

            printf("%s\t%ld\t%ld\t%ld\tdist: %ld\n",
                    fragment[COL_CHR],
                    atol(fragment[COL_POS1]),
                    atol(fragment[COL_POS2]),
                    atol(fragment[COL_MID]),
                    dist);
        }
    }
}

void print_data(FILE *out) {
    for (size_t d = 0; d < MAX_DIST; d++) {
        for (size_t l = 0; l < MAX_LENGTH; l++) {
            if (data[d][l] != 0) {
                fprintf(out, "%lu\t%zu\t%zu\n", data[d][l], d, l);
            }
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 3 && argc != 4) {
        fprintf(stderr, "Usage: <reads midpoint tsv file> <nucleosome midpoint tsv file> [file delimiter]\n");
        exit(1);
    }

    fprintf(stderr, "Start assign nucleosome midpoints\n");

    FILE *reads = fopen(argv[1], "r");

    if (reads == NULL)
    {
        DIE_PERROR(1, "Failed to open reads file: %s\n", argv[1]);
    }

    FILE *nucs = fopen(argv[2], "r");

    if (nucs == NULL)
    {
        DIE_PERROR(1, "Failed to open nucs file: %s\n", argv[2]);
    }

    char * delimiter = "\t";

    if (argc == 4) {
        delimiter = argv[3];
    }

    retrieve_data(reads, nucs, delimiter);
    print_data(stdout);

    fclose(nucs);
    fclose(reads);

    fprintf(stderr, "End assign nucleosome midpoints\n");
}