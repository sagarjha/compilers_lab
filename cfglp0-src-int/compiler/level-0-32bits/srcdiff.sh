#!/bin/bash
for f in *.hh
do
	echo $f
	diff -bB $f ../cfglp0-v2.1-combined-source-modified-march-23/level-0-32bits/$f
	echo
	echo
done

for f in *.cc
do
	echo $f
	diff -bB $f ../cfglp0-v2.1-combined-source-modified-march-23/level-0-32bits/$f
	echo
	echo
done
