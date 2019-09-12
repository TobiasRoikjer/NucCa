# Arrange paired reads
Outputs a sorted tab separated stream with a list of paired reads. The list
includes: The chromosome, the position of the first read, and the position of
the second read (combined they make a fragment). The file is sorted by
chromosome, and by first read position, both in ascending order.

Example:
Chr1	5	55
Chr1	16	60
Chr2	55	200
ChrX	45	60
