for i in 2 
do
	for j in 1 2 3 4 5 
	do
		sbatch --constraint=elves --time=04:00:00 --ntasks-per-node=$i --nodes=1 --mem-per-cpu=$((16/$i))G --partition=killable.q openmp-sbatch.sh
	done
done
