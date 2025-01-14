#!/bin/bash

Curdir=`pwd`
parentdir="$(dirname "$Curdir")"
outputdir=/fdata/hepx/store/user/lpernie/Hhh/
outputsuffix=.root
outputprefix=delphes_B3_1M_PU40_
inputfile=/fdata/hepx/store/user/taohuang/Hhh/delphes_NewB3_200k_part1_PU40_20160316_UpdateBtag_leptonW_antik4.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_NewB3_200k_part2_PU40_20160316_UpdateBtag_leptonW_antik4.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_NewB3_200k_part3_PU40_20160316_UpdateBtag_leptonW_antik4.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_NewB3_200k_part4_PU40_20160316_UpdateBtag_leptonW_antik4.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_NewB3_200k_part5_PU40_20160316_UpdateBtag_leptonW_antik4.root
parafile=/home/lpernie/Hhh/delphes/diHiggs/parametersconfig_B3PU.txt
totalevents=1000000
numjobs=40
batchjob=Delphes_splitjobs_Ana.slrm

function checkjobsstatus(){
arrayid=("$@")
echo "inptu array "${arrayid[@]}," len "${#arrayid[@]}
test=true
while $test; 
do
    test=false
    for ((i=0; i<${#arrayid[@]}; i++));
    do
	echo "$i element  "${arrayid[i]}
	squeue -u lpernie | grep ${arrayid[i]} > /dev/null
	if [ $? -eq 0 ]; then
		echo " running batch job "${arrayid[i]}
		test=true
	else
		echo " not running batch job "${arrayid[i]}
	fi
    done
    sleep 30

done

}


function checkjoboutput(){

while true; 
do
    ls | grep $1 > /dev/null
    if [ $? -eq 0 ]; then
	echo "has output of job "$1
	break
    else
	echo "fail to find output of job "$1
    fi
    sleep 30
done
}

#if [ -d "$outputdir" ]; then
  # Control will enter here if $DIRECTORY exists.
  # 	rm -rf $outputdir
#fi

mkdir -p $outputdir
eventsperjob=$((totalevents/numjobs))
for ((i=0; i<numjobs; i++)); 
do
	#replace parameter
	newparafile=$parafile"_"$i
	nstarts=$((eventsperjob*i))	
	outputfile=$outputdir$outputprefix"_"$i$outputsuffix
	echo "nEvents "$eventsperjob"  nStarts "$nstarts
	touch newparafile
	cp $parafile $newparafile
	sed -i "/nEvents/d" $newparafile
	sed -i "/nStarts/d" $newparafile
        sed -i "2i \nEvents   $eventsperjob" $newparafile
        sed -i "2i \nStarts   $nstarts" $newparafile
	cat $newparafile
	arrayjobid[i]=$(sbatch $batchjob $inputfile $outputfile $newparafile | awk 'NF>1{print $NF}')
	echo "jobid "${arrayjobid[i]}
	if [ $i -eq $((numjobs-1)) ]; then
		echo "all jobs id "${arrayjobid[@]}
		checkjoboutput ${arrayjobid[i]}
		checkjobsstatus "${arrayjobid[@]}"
		
	fi

done
