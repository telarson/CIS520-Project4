
#!/bin/bash

for i in 1 2 4 8 16
do
	sbatch --time=04:00:00 --constraint=dwarves --ntasks-per-node=$i --nodes=1 --mem-per-cpu=$((32/$i))G --partition=killable.q mpi_sbatch.sh
done
