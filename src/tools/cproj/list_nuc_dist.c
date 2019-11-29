#define _GNU_SOURCE

#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#include "utils.h"
#include "file.h"

#define COL_CHR 0
#define COL_POS1 1
#define COL_POS2 2
#define COL_MID 3

void retrieve_data(FILE *reads, FILE *nucs, char *delimiter, size_t side_index) {
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

    long read_edge = -1;
    long nuc_midpoint_low = -1;
    long nuc_midpoint_high = -1;
    long frag_len = 0;
    bool left_side_eq_chosen = false;

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
        read_edge = atol(fragment[side_index]);
        read_chrom = fragment[COL_CHR];
        frag_len = atol(fragment[COL_POS2]) - atol(fragment[COL_POS1]);

        if (strverscmp(read_chrom, nuc_chrom_low) < 0) {
            printf("%s\t%ld\t%ld\t%s\n",
                   fragment[COL_CHR],
                   frag_len,
                   atol(fragment[side_index]),
                   "NA");

            continue;
        }

        while ((nuc_midpoint_high < read_edge
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

        long dist;
        bool set = false;

        if (nuc_chrom_low != NULL && strcmp(read_chrom, nuc_chrom_low) == 0) {
            dist = read_edge - nuc_midpoint_low;

            set = true;
        } else{
		    printf("%s\t%ld\t%ld\t%s\n",
		   fragment[COL_CHR],
		   frag_len,
		   atol(fragment[side_index]),
		   "NA");
continue;
            }


        if (nuc_chrom_high != NULL && strcmp(read_chrom, nuc_chrom_high) == 0) {
            long high_dist = read_edge - nuc_midpoint_high;
            if (!set) {
                dist = high_dist;
            } else {
                if (labs(high_dist) < labs(dist)) {
                    dist = high_dist;
                } else if (labs(high_dist) == labs(dist)) {
                    if (left_side_eq_chosen) {
                        dist = high_dist;
                    }
                    left_side_eq_chosen = 1 - left_side_eq_chosen;
                }
            }
            set = true;
        } else {
		    printf("%s\t%ld\t%ld\t%s\n",
		   fragment[COL_CHR],
		   frag_len,
		   atol(fragment[side_index]),
		   "NA");
continue;
            }
/*
   if (nuc_midpoint_high - nuc_midpoint_low > 147*2 || nuc_midpoint_high - nuc_midpoint_low < 147) {
		    printf("%s\t%ld\t%ld\t%s\n",
		   fragment[COL_CHR],
		   frag_len,
		   atol(fragment[side_index]),
		   "NA");
continue;
            }
*/
        if (set) {
            printf("%s\t%ld\t%ld\t%ld\n",
                    fragment[COL_CHR],
                    frag_len,
                    atol(fragment[side_index]),
                    dist);
        } else {
            printf("%s\t%ld\t%ld\t%s\n",
                   fragment[COL_CHR],
                   frag_len,
                   atol(fragment[side_index]),
                   "NA");
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 4 && argc != 5) {
        fprintf(stderr, "%s\n%s\n",
                "Usage: <reads midpoint tsv file> <nucleosome midpoint tsv file> <l/r> [file delimiter]",
                "l/r indicates left edge of fragment or right edge of fragment");
        exit(1);
    }

    fprintf(stderr, "Start list nucleosome distances\n");

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

    unsigned int side_index;
    char *side = argv[3];

    if (strcmp("l", side) == 0) {
        side_index = COL_POS1;
    } else if (strcmp("r", side) == 0) {
        side_index = COL_POS2;
    } else {
        fprintf(stderr, "Side must be l or r, got %s\n", side);
        exit(1);
    }

    char * delimiter = "\t";

    if (argc == 5) {
        delimiter = argv[4];
    }

    retrieve_data(reads, nucs, delimiter, side_index);

    fclose(nucs);
    fclose(reads);

    fprintf(stderr, "End assign nucleosome midpoints\n");
}
