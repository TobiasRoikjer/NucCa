#!/usr/bin/env sh

echo "Enter paired samfile: "
read samfile

#wget http://hgdownload.cse.ucsc.edu/goldenPath/hg19/encodeDCC/wgEncodeUwRepliSeq/wgEncodeUwRepliSeqK562G1AlnRep1.bam
samtools view -h $samfile | head -n 100000 > t.sam && samtools view -Sb t.sam > short_paired.bam && rm t.sam