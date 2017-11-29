#!/bin/sh
make -f Makefile
./IFJcode17 <qq.txt >out.txt
./ic17int out.txt
