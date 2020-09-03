ROOT = ../../../..
include $(ROOT)/global.mk
#-include ../local.mk
HEAP ?= -heap=6384 -heap1=16384 -heap2=16384 -heap3=16384
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC5103 board and stores return code in ".exitcode"  file

.SUFFIXES:


TARGET0   = board-nmpu0-x86d
TARGET1   = board-nmpu1-x86d
#HOST      = board-host

VS ?=vs2015

.PHONY : vs2005 vs2015


vs2015:	
	premake5 vs2015
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(TARGET0).vcxproj /p:Configuration=Debug
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(TARGET1).vcxproj /p:Configuration=Debug

vs2005: 
	premake5 vs2005
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(TARGET0).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(TARGET1).vcproj

#$(HOST).vcproj $(TARGET0).vcproj $(TARGET1).vcproj:
#	premake5 vs2005

#$(HOST).vcxproj $(TARGET0).vcxproj $(TARGET1).vcxproj:
#	premake5 vs2015


run:
	$(MAKE) -j4 runtarget0  runtarget1 

runtarget0: 
	bin\x86\debug\$(TARGET0)
	
runtarget1: 
	bin\x86\debug\$(TARGET1)
	

include $(ROOT)/clean.mk

