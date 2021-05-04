#!/bin/bash -l
##$ -l h_rt=0:01:00
#SBATCH --constraint=dwarves
# #SBATCH --nodes=1 --ntasks-per-node=12

module load OpenMPI

mpirun //usr/bin/time -o /homes/jcsoutherland/proj4/output/time/time-$RANDOM.txt /homes/jcsoutherland/proj4/output/numbers/avg_mpi
