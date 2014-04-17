#!/bin/bash
mkdir output/test_files
for f in test_files/*.cs306.cfg
do
    echo $f
     ./cfglp32 -compile -d $f > output/$f.asm
done

echo
echo
echo "Bad files"
echo
for f in test_files/*.cs306.ecfg
do
    echo $f
     ./cfglp32 -compile -d $f > output/$f.asm
     echo
done

cp output/test_files/* output
rm -r output/test_files
