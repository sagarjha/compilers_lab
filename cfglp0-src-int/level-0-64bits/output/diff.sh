#!/bin/bash
for f in ./*.cfg
do
    diff ../../level-1-64bits/output/$f $f
done