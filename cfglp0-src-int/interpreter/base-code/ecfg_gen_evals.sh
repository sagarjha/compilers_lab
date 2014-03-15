#!/bin/bash
zero=0;
mkdir output/test_files
for f in test_files/*.ecfg
do
    timeout 0.2s ./cfglp -eval -d $f > output/$f.eval
    res=$?
    if [[ $res -eq $zero ]]; then
	echo
	echo $f : ---------NO ERROR---------
	echo
    fi
done
cp output/test_files/* output
rm -r output/test_files
