import pysam
import os

class BAM:
    def __init__(self, filename):
        if not os.path.exists(filename + ".bai"):
            pysam.index(filename)

        self.bam_file = pysam.AlignmentFile(filename, 'rb')

    def pair_generator(self):
        mem = {}
        #i = 0
        #maxi = 0

        for read in self.bam_file.fetch():
            if not read.is_proper_pair or read.is_duplicate \
                    or read.is_secondary or read.is_supplementary:
                continue

            query_name = read.query_name

            if query_name not in mem:
                mem[query_name] = read.pos
                #i += 1
                #maxi = max(maxi, i)
            else:
                mem_pos = mem[query_name]
                del mem[query_name]
                #i -= 1

                yield f"chr{read.reference_name}", min(read.pos, mem_pos), max(read.pos, mem_pos)