import sys
import glob
import os

d = sys.argv[1]
nuc = sys.argv[2]
to = sys.argv[3]

print(f"Reading files in dir {d}, saving to {to}", file=sys.stderr)

for f in glob.glob(f"{d}/*.fragments.tsv"):
    name = f"{os.path.basename(f).split('.')[0]}.summary.tsv"
    out_file = os.path.join(to, name)

    if os.path.exists(out_file):
        print(f"Skipping {f}, as {out_file} exists.", file=sys.stderr)
    else:
        print(f"Running: ./list_nuc_dist {f} {nuc} l > tl", file=sys.stderr)
        os.system(f"./list_nuc_dist {f} {nuc} l > tl")
        print(f"Running: ./list_nuc_dist {f} {nuc} r > tr", file=sys.stderr)
        os.system(f"./list_nuc_dist {f} {nuc} r > tr")
        os.system(f"./fletch.sh tl tr > t")
        os.system(f"Rscript Rds.R t {out_file}")
