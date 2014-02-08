#!/bin/bash
mkdir output/test_files
for f in test_files/*.cs306.cfg
do
     ./cfglp -ast -d $f > output/$f.asts
done
cp output/test_files/* output
rm -r output/test_files
