#!/bin/bash
for f in test_files/*.cs306.cfg
do
	./cfglp64 -tokens -d $f > output/$f
done
