#!/bin/bash
mkdir output/test_files
for f in test_files/*.ecfg
do
     ./cfglp -icode -lra -d $f > output/$f.icodeopt
     res=$?
    if [[ $res -eq $zero ]]; then
	echo
	echo $f : ---------NO ERROR---------
	echo
    fi
done
cp output/test_files/* output
rm -r output/test_files
