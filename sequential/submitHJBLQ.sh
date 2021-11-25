#!/bin/bash

sbatch scripts/script_HJBLQ_d100_b1.sh
sbatch scripts/script_HJBLQ_d100_b64.sh
#sbatch scripts/script_HJBLQ_d100_b128.sh
#sbatch scripts/script_HJBLQ_d100_b256.sh

sbatch scripts/script_HJBLQ_d200_b1.sh
sbatch scripts/script_HJBLQ_d200_b64.sh
#sbatch scripts/script_HJBLQ_d200_b128.sh
#sbatch scripts/script_HJBLQ_d200_b256.sh

sbatch scripts/script_HJBLQ_d300_b1.sh
sbatch scripts/script_HJBLQ_d300_b64.sh
#sbatch scripts/script_HJBLQ_d300_b128.sh
#sbatch scripts/script_HJBLQ_d300_b256.sh

