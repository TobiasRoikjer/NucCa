import csv
import sys

if (len(sys.argv) != 2 and len(sys.argv) != 3):
    print("Usage: <file> [min score]", file=sys.stderr) 
    sys.exit(1)

min_score = None

if (len(sys.argv) == 3):
    min_score = float(sys.argv[2])

print(f"file {sys.argv[1]}, min_score {min_score}", file=sys.stderr)
last_mid = None
next_mid = None
line = None

with open(sys.argv[1]) as tsvfile:
    tsvreader = csv.reader(tsvfile, delimiter="\t")
    for line2 in tsvreader:
        mid = None
        if (line is not None and len(line) > 1 and line2 is not None and len(line2) > 1):
            if (min_score is None or float(line[3]) >= min_score):
                mid = (int(line[2])+int(line[1]))//2
                next_mid = (int(line2[2])+int(line2[1]))//2

                if (last_mid is not None and abs(mid-last_mid) >= 147 and abs(mid-next_mid) >= 147):
                    print(f"{line[0]}\t{line[1]}\t{line[2]}\t{mid}")
        line = line2
        last_mid = mid


