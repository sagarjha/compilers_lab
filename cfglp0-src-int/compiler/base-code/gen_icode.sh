#!/bin/bash
mkdir output/test_files
for f in test_files/*.cs306.cfg
do
    echo $f
     ./cfglp32 -icode -d $f > output/$f.icode
done
cp output/test_files/* output
rm -r output/test_files
