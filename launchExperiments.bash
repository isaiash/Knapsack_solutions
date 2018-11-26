#!/bin/bash

for i in /shared/home/ihuerta/dataset/*
do
	qsub -pe make 7 -j yes -e ./error ./call_solver.sh greedy $i
    qsub -pe make 7 -j yes -e ./error ./call_solver.sh dp1 $i
    qsub -pe make 7 -j yes -e ./error ./call_solver.sh dp2 $i
    qsub -pe make 7 -j yes -e ./error ./call_solver.sh bf $i
    qsub -pe make 7 -j yes -e ./error ./call_solver.sh cplex $i
    qsub -pe make 7 -j yes -e ./error ./call_solver.sh memetic $i
done

