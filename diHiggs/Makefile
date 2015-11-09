# -*- Makefile -*- 

#
# This Makefile is intended for compiling cc under delphes
#
# This Makefile received very little testing, 
# any bug reports are very welcome!
#Tao Huang, email: taohuang@physics.tamu.edu
#include Makefile.arch
#LDLIBS=`root-config --glibs`
#ROOTLIBS='-lRooFit -lHtml -lMinuit -lRooFitCore -lRooStats -lHistFactory'
#DelphesDir=/home/taohuang/Herwig++/Delphes-3.2.0
DelphesDir=$(shell dirname "$(CURDIR)")
#test:
#	@echo "a"
#	@echo $(CURDIR)
#	@echo $(shell dirname "$(CURDIR)")
ROOTINCLUDE=$(shell root-config --glibs 2> /dev/null) -lTMVA
#DelphesINCLUDE=-I$(DelphesDir)/classes -I$(DelphesDir)/modules -I$(DelphesDir)/readers
#DelphesINCLUDE=-I$(DelphesDir) -I$(DelphesDir)/external 
DelphesINCLUDE=-I$(DelphesDir)
DelphesLIB=-L$(DelphesDir) -lDelphes
#Other_INCLUDE_if_necessary= 
LDFLAGS=-Wl,--no-as-needed
LDFLAGS+=`root-config --ldflags`
INCLUDE = $(DelphesINCLUDE) $(ROOTINCLUDE)
LIB=$(DelphesLIB)
#
# C++ flags
# 
CXX=g++
CXXFLAGS=-pedantic -ansi -Wall -Wno-long-long -Wno-format -Werror=uninitialized -Werror=delete-non-virtual-dtor -O2
CXXFLAGS+=`root-config --cflags`
#src file
Example_SRC = testMake.cpp 
Example_EXE = testMake.exe

#DiHiggsAnalyzer
DiHiggsAna_SRC = DiHiggsAnalyzer.cpp DiHiggstollbb.h DiHiggstollbb.C MMC.h MMC.cc
DiHiggsAna_EXE = DiHiggsAna.exe
DiHiggsVal_SRC = DiHiggsValidation.cpp DiHiggstoWWbb.h DiHiggstoWWbb.C MMC.h MMC.cc
DiHiggsVal_EXE = DiHiggsValidation.exe
#DiHiggsVal_EXE = ttbarValidation.exe

ttbarAna_SRC = ttbarAnalyzer.cpp ttbartollbb.h ttbartollbb.C MMC.h MMC.cc
ttbarAna_EXE = ttbarAna.exe
ttbarVal_SRC = ttbarValidation.cpp ttbartoWWbb.h ttbartoWWbb.C MMC.h MMC.cc
ttbarVal_EXE = ttbarValidation.exe
# test print out
#var=TEST
#PHONY:
#	@echo $(INCLUDE)
	#echo $(LIB)
#default : DiHiggs
all: DiHiggsVal
Example:
	$(CXX) $(CXXFLAGS) -g $(Example_SRC) $(LDFLAGS) $(INCLUDE) $(LIB) -o $(Example_EXE)	
DiHiggsAna:
	$(CXX) $(CXXFLAGS) -g $(DiHiggsAna_SRC) $(LDFLAGS) $(INCLUDE) $(LIB) -o $(DiHiggsAna_EXE)	
DiHiggsVal:
	$(CXX) $(CXXFLAGS) -g $(DiHiggsVal_SRC) $(LDFLAGS) $(INCLUDE) $(LIB) -o $(DiHiggsVal_EXE)	
#ttbarVal:
#	$(CXX) $(CXXFLAGS) -g $(ttbarVal_SRC) $(LDFLAGS) $(INCLUDE) $(LIB) -o $(ttbarVal_EXE)	
	
#project : DiHiggsWWBBAna.cc 
#	$(CXX) $(CXXFLAGS) -c $<
#	$(CXX) $(LDFLAGS) DiHiggsWWBBAna.o $(CXXOBJS) $(LIBS) -o DiHiggsWWBBAna

clean:
	rm -f *.exe
