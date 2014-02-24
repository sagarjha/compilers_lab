#!/bin/bash
make -f Makefile.cfglp
./make-cfg.sh
./gen_asts_cfg.sh
./gen_tokens_cfg.sh
./gen_evals_cfg.sh
cd output;\
./diff.sh;\
cd ..
