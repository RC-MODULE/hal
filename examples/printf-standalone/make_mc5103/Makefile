ROOT = ../../..
-include $(ROOT)/global.mk
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC5103 board 




ifndef MC5103
$(error ERROR: 'MC5103' environment variable is not defined!  )
endif 

PATH:=$(MC5103)/bin;$(PATH)

TARGET   = main.abs
CFG      = mc5103brd.cfg
INC_DIRS = -I"$(MC5103)/include" -I$(ROOT)/include
LIB_DIRS = -L"$(MC5103)/lib"     -L$(ROOT)/lib
LIBS     = mc5103lib.lib hal-mc5103.lib libint05.lib
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,..))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,..))
HEAP     = -heap=14384 -heap1=16384 -heap2=16384 -heap3=16384
FLAGS	 = -nmc3 -g -O0


$(TARGET): $(SRC_CPP) $(SRC_ASM)  $(CFG)  Makefile
	nmcc $(FLAGS) $(HEAP) -c$(CFG) -o$(TARGET) -m.main.map $(SRC_CPP) $(SRC_ASM)  $(INC_DIRS) $(LIB_DIRS) $(LIBS) 

run: 
	"$(MC5103)/bin/mc5103run" $(TARGET) 

rund:
	-$(MAKE) run
	pause

vs2005:
	premake5 vs2005

vs2015:
	premake5 vs2015
		
-include $(ROOT)/clean.mk	

	