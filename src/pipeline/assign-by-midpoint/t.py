import csv
import sys

with open(sys.argv[1]) as tsvfile:
    tsvreader = csv.reader(tsvfile, delimiter="\t")
    for line in tsvreader:
        if (len(line) > 1):
            print(f"{line[0]}\t{line[1]}\t{line[2]}\t{(int(line[2])+int(line[1]))//2}")


