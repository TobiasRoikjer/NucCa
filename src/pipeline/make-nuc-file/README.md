Used to parse nucleosome bed files into tsv files with midpoints.

Use filter.sh to filter the nucleosomes such that we remove those that are
within 147 of eachother.

Usage:  ./run.sh <nuc-file> [min-score] > unfiltered.tsv
        ./filter.sh unfiltered.tsv > filtered.tsv
