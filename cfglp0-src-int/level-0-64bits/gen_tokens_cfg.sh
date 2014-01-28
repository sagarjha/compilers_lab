#!/bin/bash
for f in test_files/*.cs306.cfg
do
     ./cfglp -tokens -d $f > output/$f
done
cp output/test_files/* output
rm -r output/test_files
