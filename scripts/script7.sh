#!/bin/sh 
#SBATCH -n 1			# 1 cores
#SBATCH -t 23:00:00		# 5 hours
#SBATCH -p compute      # partition name 
#SBATCH -J BSDE_job_1_procs		# sensible name for the job  
#SBATCH -A HPC_21_01207

# load up the correct modules, if required 
module load gcc  

# launch the code 
./BSDE.out HJBLQ2.txt H
