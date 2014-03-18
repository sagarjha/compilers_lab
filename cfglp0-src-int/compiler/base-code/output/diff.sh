#!/bin/bash
echo Intermediate code without optimization
echo
echo forward diff

for f in ./*.cfg.icode
do
    diff -bB $f ../../level-1-32bits/output/$f
done

echo
echo backward diff

for f in ./*.cfg.icode
do
    diff -bB ../../level-1-32bits/output/$f $f
done
