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



BOARD    = mc12101
HOST     = $(BOARD)-x86


TARGET   = main.abs
INC_DIRS = -I"$(MC12101)/include" -I$(ROOT)/include -I"$(NEURO)/include" -I$(NMPP)/include
LIB_DIRS = -L"$(MC12101)/lib" -L$(ROOT)/lib -L$(NMPP)/lib
LIBS     = mc12101lib_nm.lib libc4f.lib libc4.lib hal-mc12101.lib nmpp-nmc4f.lib
CFG      = mc12101brd-nmc0.cfg
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,../src_nm))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,../src_nm))


ALL: $(TARGET) $(HOST)
$(TARGET): $(SRC_CPP) $(SRC_ASM) $(CFG) Makefile
	nmcc -o$(TARGET) -DMC12101 -DNMC0 -m.main.map $(SRC_CPP) $(SRC_ASM)  -nmc4 -g -O0 $(INC_DIRS) $(LIB_DIRS) $(LIBS) -c$(CFG) -heap=16384 -heap1=16384 -heap2=16384 -heap3=16384
$(HOST): $(HOST).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(BOARD)-x86.vcproj



PATH:=$(MC12101)/bin;$(PATH)



run: $(TARGET) $(HOST)
	bin\debug\$(HOST)
	


clean:
	-$(OS_RM) *.abs *.elf *.ncb *.map *.elf *.suo *.user *.filters
	-$(OS_RD) bin obj
	
kill: clean
	-$(OS_RM) *.vcproj *.sln *.vcxproj 
	
$(HOST).vcproj  vs2005:	premake5.lua 
	premake5 vs2005
	