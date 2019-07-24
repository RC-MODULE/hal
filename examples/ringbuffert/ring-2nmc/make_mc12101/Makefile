ROOT = ../../../..
include $(ROOT)/global.mk
#-include ../local.mk
HEAP ?= -heap=6384 -heap1=16384 -heap2=16384 -heap3=16384
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC12101 board and stores return code in ".exitcode"  file

.SUFFIXES:


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

ALL: nmc0 nmc1

run: 
	$(MAKE) -f Makefile0 
	$(MAKE) -f Makefile1 
	mc12101run $(TARGET0) $(TARGET1)
	
run0:
	$(MAKE) -f Makefile0 run

run1:
	$(MAKE) -f Makefile1 run

$(TARGET0) nmc0:
	$(MAKE) -f Makefile0 

$(TARGET1) nmc1:
	$(MAKE) -f Makefile1

$(HOST): $(VS)
	
vs2005: $(HOST).vcproj 
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(HOST).vcproj

vs2015:	$(HOST).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(HOST).vcxproj /p:Configuration=Debug

$(HOST).vcproj:
	premake5 vs2005

$(HOST).vcxproj:
	premake5 vs2015


	
include $(ROOT)/clean.mk


	