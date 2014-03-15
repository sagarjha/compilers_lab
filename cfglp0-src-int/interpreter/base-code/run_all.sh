#!/bin/bash
echo Building the executable
make -f Makefile.cfglp
echo
echo
echo Making cfgs
./make-cfg.sh
echo
echo
echo generating tokens
./gen_tokens_cfg.sh
echo
echo
echo generating asts
./gen_asts_cfg.sh
echo
echo
echo generating eval
./gen_evals_cfg.sh
echo
echo
echo diff
cd output;\
./diff.sh;\
cd ..
echo
echo
