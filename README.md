# Nucleosome ctDNA analysis code
Usage: extract this to a (linux) server. Build the cproj files in tools/cproj using cmake.

Pipeline:
 - First create nucleosome references in the directory pipeline/make-nuc-file (read README.md in there)
 - Then create fragments from BAM files in the directory pipeline/arr-paired-reads (read README.md in there)
 - Then run one of the other pipielines (read README.md in their directories)

Example: create nucleosome reference nuc_fil.tsv. Create fragments in dir frags. Run assign-by-edge, list_all.py frags nuc_fil.tsv out

Contact: Tobias Røikjer, tobias.roikjer@post.au.dk
