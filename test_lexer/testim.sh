#!/bin/sh

RED="\033[0;31m"
NC="\033[0m"
GREEN="\033[0;32m"

for f in *.txt 
do 
    eval "./a.out $f > /dev/null 2>&1"
    if [ $? -eq 0 ]; then
        printf "${GREEN}$f passed\n"
    else 
        printf "${RED}$f failed\n"
    fi
done
