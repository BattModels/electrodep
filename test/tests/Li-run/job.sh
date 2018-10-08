#!/bin/bash
#
#SBATCH -J moosejob
#SBATCH --ntasks-per-node 64
#SBATCH -N 1
#SBATCH --time=6-00
#SBATCH -p gpu 
#SBATCH -A gpu
#SBATCH --mem=128000 
#SBATCH -o j_%j.out 
#SBATCH --mail-type=END
#SBATCH --mail-user=azeeshan@cmu.edu 

echo "Job started on `hostname` at `date`"
source ~/moose-env.sh
export OMPI_MCA_opal_event_include=poll
mpirun -np 64 ./electrodep-opt -i liqcrys.i > liqcrys.log 2>&1

echo " "
echo "Job Ended at `date`"
