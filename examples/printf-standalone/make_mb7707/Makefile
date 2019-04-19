ROOT = ../../..
-include $(ROOT)/global.mk
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MB7707 board 

.SUFFIXES:

ifndef MB7707
$(warning*** WARNING: 'MB7707' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

ifndef MB7707MAC
$(warning*** WARNING: 'MB7707MAC' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

PATH:=$(MB7707)/bin;$(PATH)

TARGET   = main.abs
CFG      = mb7707brd.cfg
INC_DIRS = -I"$(MB7707)/include" -I$(ROOT)/include
LIB_DIRS = -L"$(MB7707)/lib"     -L$(ROOT)/lib
LIBS     = mb7707lib.lib hal-mb7707.lib libint_soc.lib
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,..))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,..))
HEAP     = -heap=14384 -heap1=16384 -heap2=16384 -heap3=16384
FLAGS	 = -soc -g -O0
$(TARGET): $(SRC_CPP) $(SRC_ASM)  $(CFG)  Makefile
	nmcc $(FLAGS) $(HEAP) -c$(CFG) -o$(TARGET) -m.main.map $(SRC_CPP) $(SRC_ASM)  $(INC_DIRS) $(LIB_DIRS) $(LIBS) 

run: 
	"$(MB7707)/bin/mb7707run" -a$(MB7707MAC) -i $(TARGET) 

rund:
	-$(MAKE) run
	pause

vs2005:
	premake5 vs2005

vs2015:
	premake5 vs2015
		
-include $(ROOT)/clean.mk	

	

