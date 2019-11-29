import csv
import sys

if (len(sys.argv) != 5):
    print("Usage: <file> <chr> <min pos> <max pos>", file=sys.stderr) 
    sys.exit(1)

chrom = sys.argv[2]
pos1 = int(sys.argv[3])
pos2 = int(sys.argv[4])
hits = [0 for x in range(0, pos2-pos1)]

with open(sys.argv[1]) as tsvfile:
    tsvreader = csv.reader(tsvfile, delimiter="\t")
    for line in tsvreader:
        if (len(line) > 1):
            if (chrom == line[0] and int(line[3]) >= pos1 and int(line[3]) < pos2):
                hits[int(line[3])-pos1] += 1


for i in range(0, pos2-pos1):
    print(f"{i+pos1}\t{hits[i]}")


