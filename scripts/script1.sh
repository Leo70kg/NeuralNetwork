#!/bin/sh 
#SBATCH -n 1			# 1 cores
#SBATCH -t 05:00:00		# 5 hours
#SBATCH -p compute      # partition name 
#SBATCH -J BSDE_job_1_procs		# sensible name for the job  


# load up the correct modules, if required 
module load gcc  

# launch the code 
./BSDE.out pricing_default_risk_d200.txt P
