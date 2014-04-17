#!/bin/bash
rm -f output/*.asts output/*.toks output/*.eval output/*.icode output/*.asm output/*.opticode output/*.optasm
#rm -f test_files/*.cfg
rm -f *~
rm -f */*~
make -f Makefile.cfglp clean
