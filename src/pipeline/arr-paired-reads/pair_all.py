import sys
import glob
import os

d = sys.argv[1]
to = sys.argv[2]

print(f"Reading files in dir {d}, saving to {to}", file=sys.stderr)

for f in glob.glob(f"{d}/*.bam"):
    name = f"{os.path.basename(f).split('.')[0]}.bam.fragments.tsv"
    out_file = os.path.join(to, name)

    if os.path.exists(out_file):
        print(f"Skipping {f}, as {out_file} exists.", file=sys.stderr)
    else:
        print(f"Running: ./run.sh {f} > {out_file}", file=sys.stderr)
        os.system(f"./run.sh {f} > {out_file}")
