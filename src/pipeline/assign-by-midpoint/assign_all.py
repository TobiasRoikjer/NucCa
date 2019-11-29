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
        os.system(f"cat {f} | ./sort.sh > f")
        print(f"Running: ./assign_nuc f {nuc} > {out_file}", file=sys.stderr)
        os.system(f"./assign_nuc f {nuc} > {out_file}")
