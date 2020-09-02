ROOT=../../../..
-include $(ROOT)/global.mk
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC12101 board 




ifndef MC12101
$(warning *** WARNING: 'MC12101' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

PATH:=$(MC12101)/bin;$(PATH)

PROJECT	 = main
TARGET   = $(PROJECT).abs
CFG      = mc12101brd-nmpu0.cfg
INC_DIRS = -I"$(MC12101)/include" -I$(ROOT)/include 
LIB_DIRS = -L"$(MC12101)/lib"     -L$(ROOT)/lib
LIBS     = mc12101lib_nm_legacy.lib hal-mc12101.lib libc4.lib libint_6407.lib

SRC_DIRS =./ ..
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,$(SRC_DIRS)))

HEAPS     = -heap=14384 -heap1=16384 -heap2=16384 -heap3=16384
FLAGS	 = -nmc4 -g -O0 
MAP      = $(PROJECT).map

$(TARGET): $(SRC_CPP) $(SRC_ASM)  $(CFG)  Makefile 
	nmcc $(SRC_CPP) $(SRC_ASM) $(FLAGS) $(HEAPS) -c$(CFG) -o$(TARGET) -m$(MAP)  $(INC_DIRS) $(LIB_DIRS) $(LIBS) | errecho

run: $(TARGET) 
	mc12101run -v $(TARGET) 

rund:
	-$(MAKE) run
	pause

vs2005:
	premake5 vs2005

vs2015:
	premake5 vs2015


profile:	
	python $(ROOT)/script/profiler.py > proflist_generated.asm
	$(MAKE)
	$(OS_RM) $(TARGET)
	python $(ROOT)/script/profiler.py $(MAP) > proflist_generated.asm

	
-include $(ROOT)/clean.mk	
	SRC_DIRS += .
