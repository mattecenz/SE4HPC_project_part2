#!/bin/bash

#SBATCH --job-name=maxFib
#SBATCH --output=maxFib.out
#SBATCH --time=10:00
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=2
#SBATCH --mem-per-cpu=100M

## Add chmod +x job.sh && sbatch script.sh && squeue -u <username>

srun singularity run main.sif
