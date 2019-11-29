import sys
import glob
import os

d = sys.argv[1]
nuc = sys.argv[2]
to = sys.argv[3]

print(f"Reading files in dir {d}, and nuc file {nuc}, saving to {to}", file=sys.stderr)

for f in glob.glob(f"{d}/*.fragments.tsv"):
    name = f"{os.path.basename(f).split('.')[0]}.summary.tsv"
    out_file = os.path.join(to, name)

    if os.path.exists(out_file):
        print(f"Skipping {f}, as {out_file} exists.", file=sys.stderr)
    else:
        print(f"Running: cat {f} | LC_ALL=C sort -n -t$'\t' -k 2,2 | LC_ALL=C sort -s -V -t$'\t' -k 1,1 > fl", file=sys.stderr)

        os.system(f"cat {f} | LC_ALL=C sort -n -t$'\t' -k 2,2 | LC_ALL=C sort -s -V -t$'\t' -k 1,1 > fl")
        print(f"Running: ./list_nuc_dist fl {nuc} l > tl", file=sys.stderr)
        os.system(f"./list_nuc_dist fl {nuc} l > tl")
        os.system(f"cat {f} | LC_ALL=C sort -n -t$'\t' -k 3,3 | LC_ALL=C sort -s -V -t$'\t' -k 1,1 > fr")
        print(f"Running: ./list_nuc_dist fr {nuc} r > tr", file=sys.stderr)
        os.system(f"./list_nuc_dist fr {nuc} r > tr")
        os.system(f"./fletch.sh tl tr > t")
        os.system(f"python3 t.py t > {out_file}")
