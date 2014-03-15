#!/bin/bash
mkdir output/test_files
for f in test_files/*.cs306.cfg
do
    echo $f
     ./cfglp32 -parse -d $f > output/$f.parse
done
cp output/test_files/* output
rm -r output/test_files
