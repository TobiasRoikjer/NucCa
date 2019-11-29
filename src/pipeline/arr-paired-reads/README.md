# Arrange paired reads
Outputs a sorted tab separated stream with a list of paired reads. The list
includes: The chromosome, the position of the first read, and the position of
the second read (combined they make a fragment), and the midpoint. The file is
sorted by chromosome, and by mindpoint, both in ascending order.

Example:
Chr1	5	55  30
Chr1	16	60  38
Chr2	55	200 127
ChrX	45	60  52

Usage: python3 pair_all.py <reads-dir> <output-dir>
