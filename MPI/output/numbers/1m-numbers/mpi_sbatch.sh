#!/bin/bash -l
##$ -l h_rt=0:04:00
#SBATCH --constraint=dwarves
# #SBATCH --nodes=1 --ntasks-per-node=16

module load OpenMPI

mpirun //usr/bin/time -o /homes/jcsoutherland/proj4/output/time/1m-time/time-$RANDOM.txt /homes/jcsoutherland/proj4/output/numbers/1m-numbers/avg_mpi_1m
