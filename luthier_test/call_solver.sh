#!/bin/bash

source ~/.bashrc
#$ -S /bin/bash
# $1 executable file absolute path in host
# $2 instace directory absolute path in host
# $3 results directory absolute path in host

rm -r -f /tmp/$$.tmp
mkdir -p /tmp/$$.tmp
mkdir /tmp/$$.tmp/instances
cd /tmp/$$.tmp

#copy instances to tmp
for inst in $(ls $2)
do
	cp $2/$inst ./instances/$inst
done

exeBaseName=$(basename $1)

for inst in ./instances/*
do
	timeout -s SIGKILL 5m $1 $inst >> ./$exeBaseName
done

cp $exeBaseName $3
cd $HOME
rm -r -f /tmp/$$.tmp

