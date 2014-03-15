#!/bin/bash
cd test_files;\
for f in *.c
do
    echo $f
    cd ..
    make FILE=$f -f Makefile.cfg
    cd test_files
done