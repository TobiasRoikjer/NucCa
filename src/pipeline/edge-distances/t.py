import csv
import sys

group_count = {}

print("\tn\tcount")
with open(sys.argv[1]) as tsvfile:
    tsvreader = csv.reader(tsvfile, delimiter="\t")
    prev_pos = 0
    n = 0
    #skipped_first_line = False
    for line in tsvreader:
        #if (not skipped_first_line):
        #    skipped_first_line = True
        #    continue
        if (len(line) >= 3):
            pos = int(line[2])
            if (abs(pos-prev_pos) <= 30):
                n += 1
            else:
                if (str(n+1) not in group_count):
                    group_count[str(n+1)] = 0
                group_count[str(n+1)] += 1
                n = 0
            prev_pos = pos

for i, n in group_count.items():
    print(f"{i}\t{i}\t{n}")
