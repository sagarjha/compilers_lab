#!/bin/bash
for f in ./*.toks
do
    echo $f
    diff -bB ../../level-2-32bits/output/$f $f
done

for f in ./*.asts
do
    echo $f
    diff -bB ../../level-2-32bits/output/$f $f
done

for f in ./*.eval
do
    echo $f
    timeout 5s diff -bB ../../level-2-32bits/output/$f $f
done
