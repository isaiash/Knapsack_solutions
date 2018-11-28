#!/bin/bash
#$1 executable dir
#$2 instances zip file
#$3 qsub output dir
#$4 output results dir

# $3 and $4 contents are deleted before writing output!!!

if [ "$#" -ne 4 ]
then
	echo "arguments: exe_dir inst_dir qsubout_dir results_dir"
	exit 1
fi

exeAbsPath=$(realpath $1)
instAbsPath=$(realpath $2)
resAbsPath=$(realpath $4)
qoutAbsPath=$(realpath $3)


rm -r -f $resAbsPath
rm -r -f $qoutAbsPath

mkdir -p $resAbsPath
mkdir -p $qoutAbsPath

for exe in $(ls $1)
do
	qsub -pe make 7 -j yes -o $qoutAbsPath/$exe$inst.out call_solver.sh $exeAbsPath/$exe $instAbsPath $resAbsPath
done

