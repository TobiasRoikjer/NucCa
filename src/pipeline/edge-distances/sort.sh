#!/usr/bin/env bash

# Sort first by position in genome and then by chromosome, use the fact that is is a
#  stable sort. Chromosome sorting uses the -V flag "natural sort of (version)
#  numbers within text", which means that chr1 is first, and chr22, chrX, chrY
#  are last. LC_ALL=C used for speedup, as we assume all ASCII chars.

LC_ALL=C sort -n -t$'\t'  -k 3,3 | LC_ALL=C sort -s -V -t$'\t' -k 1,1
