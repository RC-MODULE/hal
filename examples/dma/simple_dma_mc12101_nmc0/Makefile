ROOT=../../..
include $(ROOT)/global.mk
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC5103 board and stores return code in ".exitcode"  file

.SUFFIXES:

#NMPP?=

ifndef MC12101
$(warning *** WARNING: 'MC12101' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

ifndef NMPP
$(error *** ERROR: 'NMPP' environment variable is not defined!  )
endif 

ROOT     = ../../..
PROJECT  =main
TARGET   =$(PROJECT).abs
INC_DIRS = -I"$(MC12101)/include" -I"$(NEURO)/include"  -I$(ROOT)/include
LIB_DIRS = -L"$(MC12101)/lib"  -L"$(NEURO)/lib" -L$(ROOT)/lib
LIBS     = mc12101lib_nm.lib libc4f.lib libc4.lib  hal-mc12101.lib
CFG      = mc12101brd-nmc0.cfg
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,..))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,.))
#HEAP     = -heap=0 -heap1=16384 -heap2=16384 -heap3=16384
HEAP     = -heap=0 -heap1=0 -heap2=0 -heap3=0

$(TARGET): $(SRC_CPP) $(SRC_ASM) $(CFG) Makefile 
	nmcc -o$(TARGET) -m.main.map $(SRC_CPP) $(SRC_ASM) -nmc4 -g -O0 $(INC_DIRS) $(LIB_DIRS) $(LIBS) -c$(CFG) $(HEAP)
#                                                                                         [L00]:64kB   [L01]:64kB  [G00]:64kB   [G01]:64kB


PATH:=$(MC12101)/bin;$(PATH)

	
run: $(TARGET)
	mc12101run $(TARGET)

clean:
	-rm -f *.abs *.elf .*  *.o

skip:
	@echo SDK missing. Compilation is skipped!
	
reset:
	reset12101	

