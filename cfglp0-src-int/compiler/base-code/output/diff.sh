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
echo asm
echo
echo

# for f in *.optasm
# do
# 	echo $f
# 	diff -bB $f ../../level-1-32bits/output/$f
# done
