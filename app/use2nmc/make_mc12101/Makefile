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

TARGET0  = main0_.abs
TARGET1  = main1_.abs

ALL: $(TARGET0) $(TARGET1) $(HOST)

$(TARGET0):
	$(MAKE) -f Makefile0 

$(TARGET1):
	$(MAKE) -f Makefile1
	
$(HOST): $(HOST).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(BOARD)-x86.vcproj


PATH:=$(MC12101)/bin;$(PATH)

run: $(TARGET0) $(TARGET1) $(HOST)
	bin\debug\$(HOST)
	
clean:
	-$(OS_RM) *.abs *.elf *.ncb *.map *.elf *.suo *.user *.filters
	-$(OS_RD) bin obj
	
kill: clean
	-$(OS_RM) *.vcproj *.sln *.vcxproj
	
vs2005:	premake5.lua $(HOST).vcproj

$(HOST).vcproj:
	premake5 vs2005
	