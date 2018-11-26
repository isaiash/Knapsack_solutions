#!/bin/bash

source ~/.bashrc
# It receives two parameters:
# $1 algorithm's name to use for solve
# $2 instance filepath
rm -r -f /tmp/$$.tmp
mkdir -p /tmp/$$.tmp
cd /tmp/$$.tmp
cp $2 ./example
/shared/home/ihuerta/Knapsack_solutions/executables/$1 example> $2
cp $2 /shared/home/ihuerta/Knapsack_solutions/outputs/$1
cd $HOME
rm -r -f /tmp/$$.tmp

