#!/usr/bin/env python

import sys
sys.path.append("../../tools/pyproj/src")

import bam

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Prints all pairs of reads in a bam file.\nUsage: <bamfile>", file=sys.stderr)
        sys.exit(1)

    bam = bam.BAM(sys.argv[1])

    for chrom, pos1, pos2 in bam.pair_generator():
        print(f"{chrom}\t{pos1}\t{pos2}")