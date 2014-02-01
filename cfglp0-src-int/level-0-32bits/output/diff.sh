!/bin/bash
for f in ./*.cfg
do
    diff ../../level-1-32bits/output/$f $f
done
