#!/bin/bash -l

#SBATCH -A g2019005
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=carmen.jing.lee@gmail.com
#SBATCH -p node -N 4 -n 64
#SBATCH -t 30:00


module load gcc openmpi

path=/proj/g2019005/nobackup/qsort_indata/
No_N=4
No_n=64
executable=qsortIsend
for f in input125000000.txt input250000000.txt input500000000.txt
do
	for s in 1 2 3
	do
		for i in 1 2 3
		do
			echo -p node -N $No_N -n $No_n
			echo The executable: $executable
			echo input file name $f 
			echo pivot option $s
			mpirun $executable $path$f outputfile.txt $s
		done
	done
done




