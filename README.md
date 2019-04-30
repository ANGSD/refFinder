[![Build Status](https://travis-ci.org/ANGSD/refFinder.svg?branch=master)](https://travis-ci.org/ANGSD/refFinder)
refFinder
=========

Program/lib to extract bases from positions in fastafiles.

notes 5march 2015
=========
copied latest version of bgzf faidx and friends from htslib to make work on latest bgzip files from SAMtools


Examples:
1)  head plink.bim |./refFinder/refFinder pseudohaploid.fa.gz plink full >mytped.tped
2) echo "chr7 10010" | ./refFinder/refFinder hg19.fa.gz 
