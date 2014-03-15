#!/bin/bash
echo tokens
echo forward diff

for f in ./*.cfg.toks
do
    diff -bB $f ../../level-3-32bits/output/$f
done

echo
echo backward diff

for f in ./*.cfg.toks
do
    diff -bB ../../level-3-32bits/output/$f $f
done

echo
echo
echo asts
echo forward diff

for f in ./*.cfg.asts
do
    diff -bB $f ../../level-3-32bits/output/$f
done

echo
echo backward diff
for f in ./*.cfg.asts
do
    diff -bB ../../level-3-32bits/output/$f $f
done

echo
echo
echo eval
echo forward diff

for f in ./*.cfg.eval
do
    timeout 5s diff -bB $f ../../level-3-32bits/output/$f
done

echo
echo backward diff

for f in ./*.cfg.eval
do
    timeout 5s diff -bB ../../level-3-32bits/output/$f $f
done
