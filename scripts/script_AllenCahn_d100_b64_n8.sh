#!/bin/sh 
#SBATCH -n 8			# 4 cores
#SBATCH -t 23:00:00		# 1 hours
#SBATCH -p compute      # partition name 
#SBATCH -J BSDE_job_4_procs		# sensible name for the job  
#SBATCH -A HPC_21_01207
#SBATCH --output=/home/users/mschpc/2019/zuoy/HPC/NeuralNetwork/output/slurm-%j.out


# load up the correct modules, if required 
module load gcc openmpi  

# launch the code 
mpirun ./BSDE.out AllenCahn_d100_b64.txt A
