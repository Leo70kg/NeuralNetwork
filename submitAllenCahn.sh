#!/bin/bash

sbatch scripts/script_AllenCahn_d100_b1.sh
sbatch scripts/script_AllenCahn_d100_b64.sh
sbatch scripts/script_AllenCahn_d100_b128.sh

sbatch scripts/script_AllenCahn_d200_b1.sh
sbatch scripts/script_AllenCahn_d200_b64.sh
sbatch scripts/script_AllenCahn_d200_b128.sh

sbatch scripts/script_AllenCahn_d300_b1.sh
sbatch scripts/script_AllenCahn_d300_b64.sh
sbatch scripts/script_AllenCahn_d300_b128.sh

