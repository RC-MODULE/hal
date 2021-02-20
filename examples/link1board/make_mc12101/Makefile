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

TARGET0  = main0.abs
TARGET1  = main1.abs

VS ?= vs2015 
#PATH:=$(MC12101)/bin;$(PATH)
all: $(TARGET0) $(TARGET1) 

nm0 $(TARGET0): 
	$(MAKE) -f Makefile0 

nm1 $(TARGET1):
	$(MAKE) -f Makefile1

run: $(TARGET0) $(TARGET1)
	mc12101run $(TARGET0) $(TARGET1)
	
include $(ROOT)/clean.mk
