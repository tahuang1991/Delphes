#!/bin/bash

Curdir=`pwd`
parentdir="$(dirname "$Curdir")"
outputdir=/home/lpernie/Hhh/delphes/diHiggs/Output/
outputsuffix=.root
outputprefix=delphes_tt_4M_PU40_Wtobtaumu_
inputfile=/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_0.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_1.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_2.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_3.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_4.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_5.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_6.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_7.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_8.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_9.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_10.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_11.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_12.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_13.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_14.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_15.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_16.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_17.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_18.root,/fdata/hepx/store/user/taohuang/Pheno/TTbar/delphes_ttbar_Wtomu_PU40_20160625_Btag_antik4_19.root
#inputfile=/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_0.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_1.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_2.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_3.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_4.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_5.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_6.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_7.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_8.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_9.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_10.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_11.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_12.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_13.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_14.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_15.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_16.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_17.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_18.root,/fdata/hepx/store/user/taohuang/Hhh/delphes_ttbar_4M_PU40_20160407_Btag_antik4_Wtomutau_11383680_19.root
parafile=/home/lpernie/Hhh/delphes/diHiggs/parametersconfig_ttPU.txt
totalevents=4000000
numjobs=120
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
