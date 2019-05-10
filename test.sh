#!/bin/bash -l

#SBATCH -A g2019005
#SBATCH -p node -N 8 -n 32
#SBATCH -t 10:00

module load gcc openmpi

#file=result.txt
path=/proj/g2019005/nobackup/qsort_indata/
#touch $file
f=input125000000.txt

mpirun quicksort $path$f outputfile.txt $s



