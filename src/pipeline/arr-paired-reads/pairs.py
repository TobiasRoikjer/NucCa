#! /usr/bin/env python

import sys
sys.path.append("../../tools/pyproj/src")

import bam

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Prints all pairs of reads in a bam file.\nUsage: <bamfile>", file=sys.stderr)
        sys.exit(1)

    bam = bam.BAM(sys.argv[1])

    i = 0

    for chrom, pos1, pos2 in bam.pair_generator():
        i += 1
        print(f"{chrom}\t{pos1}\t{pos2}\t{(pos1+pos2)//2}")

        if i % 1000000 == 0:
            print(f"Progress: {i} pairs...", file=sys.stderr)
