ROOT = ../../..
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

BOARD    = mc12101
HOST     = $(BOARD)-x86

TARGET0  = main0.abs
TARGET1  = main1.abs

ALL: $(TARGET0) $(TARGET1)

ifdef NMCC
IF_NMCC = NMCC=y
endif


$(TARGET0):
	$(MAKE) -f Makefile0 $(IF_NMCC)

$(TARGET1):
	$(MAKE) -f Makefile1 $(IF_NMCC)
	


PATH:=$(MC12101)/bin;$(PATH)

run: $(TARGET0) $(TARGET1)
	mc12101run $(TARGET0) $(TARGET1)
	
reset:
	mc12101reset
		
clean:
	-$(OS_RM) *.abs *.elf *.ncb *.map *.elf *.suo *.user *.filters
	-$(OS_RD) bin obj
	
kill: clean
	-$(OS_RM) *.vcproj *.sln *.vcxproj 
	
vs2005:	
	premake5 vs2005
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(HOST).vcproj

vs2015:	
	premake5 vs2015
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(HOST).vcxproj

	