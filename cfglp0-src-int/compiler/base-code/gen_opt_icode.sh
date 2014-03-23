#!/bin/bash
mkdir output/test_files
for f in test_files/*.cs306.cfg
do
    echo $f
     ./cfglp32 -icode -lra -d $f > output/$f.opticode
done

echo
echo
echo "Bad files"
echo
for f in test_files/*.cs306.ecfg
do
    echo $f
     ./cfglp32 -icode -lra -d $f > output/$f.opticode
     echo
done

cp output/test_files/* output
rm -r output/test_files
