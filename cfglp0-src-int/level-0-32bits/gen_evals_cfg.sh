#!/bin/bash
mkdir output/test_files
for f in test_files/*.cs306.cfg
do
    echo $f
    timeout 5s ./cfglp -eval -d $f > output/$f.eval
    # if !($? = 0) then
    # 	rm output/$f.eval
    # fi
done
cp output/test_files/* output
rm -r output/test_files
