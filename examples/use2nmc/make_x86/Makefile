ROOT = ../../..
include $(ROOT)/global.mk
#-include ../local.mk
HEAP ?= -heap=6384 -heap1=16384 -heap2=16384 -heap3=16384
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC5103 board and stores return code in ".exitcode"  file

.SUFFIXES:


TARGET0   = board-target0
TARGET1   = board-target1
HOST      = board-host

VS ?=vs2015
ALL: $(VS)
.PHONY = vs2005 vs2015


$(TARGET0) $(TARGET1) $(HOST): $(VS)
	
vs2005: $(HOST).vcproj $(TARGET0).vcproj $(TARGET1).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(HOST).vcproj	
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(TARGET0).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(TARGET1).vcproj
	

vs2015:	$(HOST).vcxproj $(TARGET0).vcxproj $(TARGET1).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(HOST).vcxproj    /p:Configuration=Debug
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(TARGET0).vcxproj /p:Configuration=Debug
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(TARGET1).vcxproj /p:Configuration=Debug

$(HOST).vcproj $(TARGET0).vcproj $(TARGET1).vcproj:
	premake5 vs2005

$(HOST).vcxproj $(TARGET0).vcxproj $(TARGET1).vcxproj:
	premake5 vs2015


run: 
	$(MAKE) -j4 runhost runtarget0  runtarget1 

runhost: $(HOST)
	bin\debug\$(HOST)
	
runtarget0: $(TARGET0) 
	bin\debug\$(TARGET0)
	
runtarget1: $(TARGET1) 
	bin\debug\$(TARGET1)
	

include $(ROOT)/clean.mk

