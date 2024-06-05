#!/bin/bash

#SBATCH --job-name=matmul
#SBATCH --output=matmul.out
#SBATCH --time=00:10
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=100M


singularity run --bind "$TMPDIR" ./main.sif
