ROOT = ../../../..
include $(ROOT)/global.mk
#-include ../local.mk
HEAP ?= -heap=6384 -heap1=16384 -heap2=16384 -heap3=16384
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC5103 board and stores return code in ".exitcode"  file

.SUFFIXES:


HOST      = test_virtuale

VS ?=vs2015
ALL: $(VS)
.PHONY = vs2005 vs2015


$(HOST): $(VS)
	
vs2005: $(HOST).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(HOST).vcproj	
	

vs2015:	$(HOST).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(HOST).vcxproj    /p:Configuration=Debug

$(HOST).vcproj:
	premake5 vs2005

$(HOST).vcxproj:
	premake5 vs2015


run: 
	$(MAKE) -j4 runhost

runhost: $(HOST)
	bin\debug\$(HOST)
	

include $(ROOT)/clean.mk

