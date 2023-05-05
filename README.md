[![Build Status](https://travis-ci.org/ANGSD/refFinder.svg?branch=master)](https://travis-ci.org/ANGSD/refFinder)
refFinder

## [![Build Status](https://github.com/ANGSD/refFinder/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/ANGSD/refFinder/actions/workflows/c-cpp.yml) 
=========

Program/lib to extract bases from positions in fastafiles.

notes 5march 2015
=========
copied latest version of bgzf faidx and friends from htslib to make work on latest bgzip files from SAMtools

notes 30 april 2019 (added plink output)
=========
Examples:
1)  head plink.bim |./refFinder/refFinder pseudohaploid.fa.gz plink full >mytped.tped
2) echo "chr7 10010" | ./refFinder/refFinder hg19.fa.gz 

notes 5may 2023
=========
Updated Makefile so it would compile on most recent compilers. Updated from travis to actions.
Another usefull example of how to run

3) echo "chr7:100-150"|./refFinder/refFinder hg19.fa.gz 
