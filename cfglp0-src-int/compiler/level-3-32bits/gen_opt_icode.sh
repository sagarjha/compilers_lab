#!/bin/bash
mkdir output/test_files
for f in test_files/*.cs306.cfg
do
     ./cfglp32 -icode -lra -d $f > output/$f.opticode
done

for f in test_files/*.cs306.ecfg
do
     ./cfglp32 -icode -lra -d $f > output/$f.opticode
done
cp output/test_files/* output
rm -r output/test_files
