#!/bin/bash
mkdir output/test_files
for f in test_files/*.ecfg
do
    echo $f
     ./cfglp -tokens -d $f > output/$f.parse
done
cp output/test_files/* output
rm -r output/test_files
