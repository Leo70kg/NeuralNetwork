#!/bin/sh 
#SBATCH -n 4			# 4 cores
#SBATCH -t 05:00:00		# 1 hours
#SBATCH -p compute      # partition name 
#SBATCH -J BSDE_job_4_procs		# sensible name for the job  


# load up the correct modules, if required 
module load gcc openmpi  

# launch the code 
mpirun ./BSDE.out pricing_default_risk.txt
