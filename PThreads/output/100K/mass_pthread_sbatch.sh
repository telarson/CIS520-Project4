#!/bin/bash

for j in 1 2 3 4 5
do
	sbatch --constraint=elves --time=04:00:00 --ntasks-per-node=16 --nodes=1 --mem-per-cpu=$((16/16))G --partition=killable.q pthread_sbatch.sh
done
