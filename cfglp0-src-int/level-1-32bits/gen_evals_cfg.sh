#!/bin/bash
mkdir output/test_files
for f in test_files/*.cs306.cfg
do
    echo $f
    `timeout 5s ./cfglp32 -eval -d $f > output/$f.eval`
done
cp output/test_files/* output
rm -r output/test_files
