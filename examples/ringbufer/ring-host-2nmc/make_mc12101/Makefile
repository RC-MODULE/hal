ROOT = ../../../..
include $(ROOT)/global.mk
#-include ../local.mk
HEAP ?= -heap=6384 -heap1=16384 -heap2=16384 -heap3=16384
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC12101 board and stores return code in ".exitcode"  file

.SUFFIXES:
PROJECT=ring-host-2nmc

CONFIGURATION ?= Release
OPT ?= -O2
ifdef DEBUG
OPT = -O0 -g 
CONFIGURATION =Debug
endif



ifndef MC12101
$(warning *** WARNING: 'MC12101' environment variable is not defined! Board SDK might not have installed correctly. )
endif 
.PHONY : vs2005 vs2015

BOARD    = mc12101
HOST     = $(BOARD)-x86
TARGET0  = main0.abs
TARGET1  = main1.abs
VS      ?= vs2015
PATH:=$(MC12101)/bin;$(PATH)

ALL: $(TARGET0) $(TARGET1) $(HOST)

$(TARGET0) nm0 nmc0:
	$(MAKE) -f Makefile0 

$(TARGET1) nm1 nmc1:
	$(MAKE) -f Makefile1

run: $(TARGET0) $(TARGET1) $(HOST)
	bin\debug\$(HOST)
	
#========================== Visual Studio build ===================

.PHONY : vs2005 vs2015 run $(VS)
	
vs2005: $(PROJECT).sln
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).sln  "$(CONFIGURATION)|Win32"
	

vs2015:	$(PROJECT).sln
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).sln /t:Build /p:Configuration=$(CONFIGURATION)

	
$(PROJECT).sln:	premake5.lua 
	premake5 $(MAKECMDGOALS) 


	
include $(ROOT)/clean.mk


	