#!/bin/bash

Curdir=`pwd`
parentdir="$(dirname "$Curdir")"
outputdir=/fdata/hepx/store/user/taohuang/Hhh/Delphes_leptonW_antik4_allReco_22/
outputsuffix=.root
outputprefix=DiHiggs_WWbb_1M_B3_allReco_22
#inputfile=/fdata/hepx/store/user/taohuang/Hhh/delphes330_B3_10k_PU0_1020_btagv3_6969448.root
#inputfile=/fdata/hepx/store/user/taohuang/Hhh/delphes_B3_1M_PU0_1015_Btag_leptonW_6969448.root
inputfile=/fdata/hepx/store/user/taohuang/Hhh/delphes_B3_1M_PU0_1105_Btag_leptonW_antik4_6969448.root
parafile=/home/taohuang/delphes/diHiggs/parametersconfig.txt
totalevents=1000000
numjobs=10
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
	squeue -u taohuang | grep ${arrayid[i]} > /dev/null
	if [ $? -eq 0 ]; then
		echo " running batch job "${arrayid[i]}
		test=true
	else
		echo " not running batch job "${arrayid[i]}
	fi
    done
    sleep 300

done

}

function clearoutput(){
arrayid=("$@")
echo "inptu array "${arrayid[@]}," len "${#arrayid[@]}
    for ((i=0; i<${#arrayid[@]}; i++));
    do
	echo "$i element  "${arrayid[i]}
	ls | rm "batchjobs_"*${arrayid[i]}*.err "batchjobs_"*${arrayid[i]}*.out
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

if [ -d "$outputdir" ]; then
  # Control will enter here if $DIRECTORY exists.
   	rm -rf $outputdir
fi

mkdir $outputdir
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
		clearoutput "${arrayjobid[@]}"
		
	fi

done
