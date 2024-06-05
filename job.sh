#!/bin/bash

#SBATCH --job-name=matmul
#SBATCH --output=matmul.out
#SBATCH --time=00:10
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=100M


module load singularity

SINGULARITY_IMAGE=main.sif

export TMPDIR=$HOME/tmp
mkdir -p $TMPDIR

singularity exec --bind $TMPDIR:$TMPDIR $SINGULARITY_IMAGE bash -c "export OMPI_MCA_tmpdir_base=$TMPDIR && mpirun -np 2 /project/build/main"
