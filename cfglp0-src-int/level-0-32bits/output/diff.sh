#!/bin/bash
for f in ./*.toks
do
    echo $f
    diff ../../level-1-32bits/output/$f $f
done

for f in ./*.asts
do
    echo $f
    diff ../../level-1-32bits/output/$f $f
done
