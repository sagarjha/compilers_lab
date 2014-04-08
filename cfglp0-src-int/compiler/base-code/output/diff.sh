#!/bin/bash
echo tokens
echo
echo
for f in *.cfg.toks
do
	echo $f
	diff -bB $f ../../level-2-32bits/output/$f
done

echo
echo
echo icode
echo
echo
for f in *.cfg.icode
do
	echo $f
	diff -bB $f ../../level-2-32bits/output/$f
done

echo
echo
echo opticode
echo
echo
for f in *.cfg.opticode
do
	echo $f
	diff -bB $f ../../level-2-32bits/output/$f
done

echo
echo
echo optasm
echo
echo

for f in *.cfg.optasm
do
	echo $f
	diff -bB $f ../../level-2-32bits/output/$f
done
