#!/bin/bash
make -f Makefile.cfglp clean
make -f Makefile.cfg clean
rm -f output/*.asts output/*.toks output/*.eval
rm -f test_files/*.cfg
rm -f *~
rm -f */*~
