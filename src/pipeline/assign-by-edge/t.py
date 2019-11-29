import csv
import sys

MAX_DIST = 1000

largerdata = 0
data = [0 for x in range(MAX_DIST*2)] 

with open(sys.argv[1]) as tsvfile:
    tsvreader = csv.reader(tsvfile, delimiter="\t")

    for line in tsvreader:
        if (len(line) >= 3):
            d1 = None
            d2 = None
            if ((line[1]).strip()!="NA"):
                d1 = int(line[1].strip())
            if ((line[2]).strip()!="NA"):
                d2 = int(line[2].strip())

            if (d1 is not None and abs(d1) < MAX_DIST):
                data[d1+MAX_DIST] += 1
            else:
                largerdata += 1
            if (d2 is not None and abs(d2) < MAX_DIST):
                data[d2+MAX_DIST] += 1
            else:
                largerdata += 1

for x in range(MAX_DIST*2):
    if (data[x] != 0):
        print(f"{x-MAX_DIST}\t{data[x]}")


print(f"NA\t{largerdata}")
