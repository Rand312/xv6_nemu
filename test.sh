#!/bin/bash
 cd xv6
 make
 ./trans.sh
 cd ../nemu
 make run
 