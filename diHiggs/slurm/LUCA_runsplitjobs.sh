#!/bin/bash

#cp LUCA_runsplitjobs.slrm LUCA_runsplitjobs1.slrm
rm -rf LUCA_runsplitjobs1.slrm LUCA_runsplitjobs2.slrm LUCA_runsplitjobs3.slrm LUCA_runsplitjobs4.slrm LUCA_runsplitjobs5.slrm
sed 's/LUCA_splitjobs_B3PU/LUCA_splitjobs_B3PU1/g' LUCA_runsplitjobs.slrm > LUCA_runsplitjobs1.slrm
sed 's/LUCA_splitjobs_B3PU/LUCA_splitjobs_B3PU2/g' LUCA_runsplitjobs.slrm > LUCA_runsplitjobs2.slrm
sed 's/LUCA_splitjobs_B3PU/LUCA_splitjobs_B3PU3/g' LUCA_runsplitjobs.slrm > LUCA_runsplitjobs3.slrm
sed 's/LUCA_splitjobs_B3PU/LUCA_splitjobs_B3PU4/g' LUCA_runsplitjobs.slrm > LUCA_runsplitjobs4.slrm
sed 's/LUCA_splitjobs_B3PU/LUCA_splitjobs_B3PU5/g' LUCA_runsplitjobs.slrm > LUCA_runsplitjobs5.slrm
sbatch LUCA_runsplitjobs1.slrm 
sbatch LUCA_runsplitjobs2.slrm 
sbatch LUCA_runsplitjobs3.slrm 
sbatch LUCA_runsplitjobs4.slrm 
sbatch LUCA_runsplitjobs5.slrm 
