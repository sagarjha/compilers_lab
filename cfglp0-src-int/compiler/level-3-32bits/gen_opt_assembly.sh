#!/bin/bash
mkdir output/test_files
for f in test_files/*.cs306.cfg
do
     ./cfglp32 -compile -lra -d $f > output/$f.optasm
done

for f in test_files/*.cs306.ecfg
do
     ./cfglp32 -compile -lra -d $f > output/$f.optasm
done
cp output/test_files/* output
rm -r output/test_files
