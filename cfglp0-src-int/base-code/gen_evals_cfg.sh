#!/bin/bash
zero=0;
mkdir output/test_files
for f in test_files/*.cs306.cfg
do
    echo $f
    timeout 0.2s ./cfglp -eval -d $f > output/$f.eval
    res=$?
    #if [[ $res -ne $zero ]]; then
	#rm output/$f.eval
    #fi
done
cp output/test_files/* output
rm -r output/test_files
