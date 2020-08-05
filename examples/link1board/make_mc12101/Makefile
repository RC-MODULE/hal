ROOT = ../../..
include $(ROOT)/global.mk
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

VS ?= vs2015 
#PATH:=$(MC12101)/bin;$(PATH)
all: $(TARGET0) $(TARGET1)  $(HOST)

nm0 $(TARGET0): 
	$(MAKE) -f Makefile0 

nm1 $(TARGET1):
	$(MAKE) -f Makefile1
	
host $(HOST): $(VS)

proflist: proflist0 proflist1
proflist0:
	$(MAKE) -f Makefile0 proflist
proflist1:
	$(MAKE) -f Makefile1 proflist
	
	
vs2005: $(HOST).vcproj 
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(HOST).vcproj

vs2015:	$(HOST).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(HOST).vcxproj /p:Configuration=Debug

$(HOST).vcproj:
	premake5 vs2005

$(HOST).vcxproj:
	premake5 vs2015


run: $(TARGET0) $(TARGET1) $(HOST)
	bin\debug\$(HOST)
	
include $(ROOT)/clean.mk
