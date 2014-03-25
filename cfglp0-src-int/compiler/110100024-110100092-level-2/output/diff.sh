#!/bin/bash
echo icode
echo
echo
for f in *.opticode
do
	echo $f
	diff -bB $f ../../level-1-32bits/output/$f
done

echo
echo
echo asm
echo
echo

for f in *.optasm
do
	echo $f
	diff -bB $f ../../level-1-32bits/output/$f
done
