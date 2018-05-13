#!/bin/bash

#Title           :assignment1_update_verifier.sh
#description     :This script will update the verifier scripts for assignment1.
#Author		     :Swetank Kumar Saha <swetankk@buffalo.edu>
#Version         :1.0
#====================================================================================

cd grader
wget --no-check-certificate -r --no-parent -nH --cut-dirs=3 -R index.html https://ubwins.cse.buffalo.edu/cse-489_589/grader/
cd ..
