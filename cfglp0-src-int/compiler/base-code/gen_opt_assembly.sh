#!/bin/bash
mkdir output/test_files
for f in test_files/*.cs306.cfg
do
    echo $f
     ./cfglp32 -compile -lra -d $f > output/$f.optasm
done

echo
echo
echo "Bad files"
echo
for f in test_files/*.cs306.ecfg
do
    echo $f
     ./cfglp32 -compile -lra -d $f > output/$f.optasm
     echo
done

cp output/test_files/* output
rm -r output/test_files
