ROOT = ../../..
-include $(ROOT)/global.mk
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC12101 board 




ifndef MC12101
$(warning *** WARNING: 'MC12101' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

PATH:=$(MC12101)/bin;$(PATH)


TARGET   = main1.abs
INC_DIRS = -I"$(MC12101)/include" -I$(ROOT)/include 
LIB_DIRS = -L"$(MC12101)/lib"     -L$(ROOT)/lib
LIBS     = mc12101lib_nm.lib hal-mc12101.lib  libc4f.lib libc4.lib libint_6407.lib
CFG      = mc12101brd-nmc1.cfg
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,..))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,..))
HEAP     = -heap=14384 -heap1=16384 -heap2=16384 -heap3=16384
FLAGS	 = -nmc4 -g -O0 


$(TARGET): $(SRC_CPP) $(SRC_ASM)  $(CFG)  Makefile
	nmcc $(FLAGS) $(HEAP) -c$(CFG) -o$(TARGET) -m.main.map $(SRC_CPP) $(SRC_ASM)  $(INC_DIRS) $(LIB_DIRS) $(LIBS) 

run: 
	mc12101run -p -v $(TARGET) 

rund:
	-$(MAKE) run
	pause

vs2005:
	premake5 vs2005

vs2015:
	premake5 vs2015
		
-include $(ROOT)/clean.mk	
	