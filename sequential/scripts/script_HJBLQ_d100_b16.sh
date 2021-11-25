#!/bin/sh 
#SBATCH -n 1			# 1 cores
#SBATCH -t 2-23:00:00		# 5 hours
#SBATCH -p compute      # partition name 
#SBATCH -J BSDE_job_1_procs		# sensible name for the job  
#SBATCH -A HPC_21_01207			# project code
#SBATCH --output=/home/users/mschpc/2019/zuoy/HPC/NeuralNetwork/sequential/output/slurm-%j.out

# load up the correct modules, if required 
module load gcc  

# launch the code 
./BSDE_sequential HJBLQ_d100_b16.txt H
