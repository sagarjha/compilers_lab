#!/bin/bash
for f in *.opticode
do
	echo
	echo $f
	diff -bB $f ../../level-1-32bits/output/$f
	echo
done
