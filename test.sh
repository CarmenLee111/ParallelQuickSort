#!/bin/bash -l

#SBATCH -A g2019005
#SBATCH -t 20:00

module load gcc openmpi

file=result.txt
path=/proj/g2019005/nobackup/qsort_indata/
touch $file

for f in input125000000.txt input250000000.txt #input500000000.txt input1000000000.txt input2000000000.txt backwards125000000.txt backwards2000000000.txt
do 
	for c in 32 64 128 256
	do		
		for s in 1 2 3
		do
			echo number of cores: $c | tee -a $file
			echo input file: $f >> $file 
			(mpirun -n $c quicksort $path$f outputfile.txt $s) >> $file 
		done
	done 
done



