@echo off
make clean -f make.library.ctr
make -f make.library.ctr
make clean -f make.library.ntr
make -f make.library.ntr
make clean -f make.library.nx
make -f make.library.nx