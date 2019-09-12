#!/usr/bin/env bash

# Sort first by position 1 and then by chromosome, use the fact that is is a
#  stable sort. Chromosome sorting uses the -V flag "natural sort of (version)
#  numbers within text", which means that chr1 is first, and chr22, chrX, chrY
#  are last. LC_ALL=C used for speedup, as we assume all ASCII chars.

python3 pairs.py "$1" | LC_ALL=C sort -n -t$'\t'  -k2 | LC_ALL=C sort -V -t$'\t' -k1