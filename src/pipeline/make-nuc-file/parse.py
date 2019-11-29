import csv
import sys

if (len(sys.argv) != 2 and len(sys.argv) != 3):
    print("Usage: <file> [min score]", file=sys.stderr) 
    sys.exit(1)

min_score = None

if (len(sys.argv) == 3):
    min_score = float(sys.argv[2])

print(f"file {sys.argv[1]}, min_score {min_score}", file=sys.stderr)

with open(sys.argv[1]) as tsvfile:
    tsvreader = csv.reader(tsvfile, delimiter="\t")
    for line in tsvreader:
        if (len(line) > 1):
            if (min_score is None or float(line[3]) >= min_score):
                print(f"{line[0]}\t{line[1]}\t{line[2]}\t{(int(line[2])+int(line[1]))//2}")


