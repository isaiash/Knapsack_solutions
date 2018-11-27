#!/bin/bash

source ~/.bashrc
#$ -S /bin/bash
# $1 executable file absolute path in host
# $2 instace zip file absolute path in host
# $3 results directory absolute path in host

rm -r -f /tmp/$$.tmp
mkdir -p /tmp/$$.tmp
mkdir /tmp/$$.tmp/instances
cd /tmp/$$.tmp

cp $2 /tmp/$$.tmp
unzip -q -j /tmp/$$.tmp/$(basename $2) -d /tmp/$$.tmp/instances

exeBaseName=$(basename $1)

for inst in ./instances/*
do
	timeout -s SIGKILL 5m $1 $inst >> ./$exeBaseName
done

cp $exeBaseName $3
cd $HOME
rm -r -f /tmp/$$.tmp

