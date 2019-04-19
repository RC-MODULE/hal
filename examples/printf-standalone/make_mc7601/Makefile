ROOT = ../../..
-include $(ROOT)/global.mk
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC7601 board 




ifndef MC7601
$(error *** ERROR: 'MC7601' environment variable is not defined!  )
endif 

PATH:=$(MC7601)/bin;$(PATH)
TARGET   = main.abs
CFG      = mc7601brd.cfg
INC_DIRS = -I"$(MC7601)/include" -I$(ROOT)/include
LIB_DIRS = -L"$(MC7601)/lib" -L$(ROOT)/lib
LIBS     = hal-mc7601.lib mc7601_ll_nm.lib libint_soc.lib  
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,..))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,..))
HEAP     = -heap=14384 -heap1=16384 -heap2=16384 -heap3=16384
FLAGS	 = -soc -g -O0


$(TARGET): $(SRC_CPP) $(SRC_ASM)  $(CFG)  Makefile
	nmcc $(FLAGS) $(HEAP) -c$(CFG) -o$(TARGET) -m.main.map $(SRC_CPP) $(SRC_ASM)  $(INC_DIRS) $(LIB_DIRS) $(LIBS) 




run: $(TARGET)
	mc7601run $(TARGET)

rund:
	-$(MAKE) run
	pause

vs2005:
	premake5 vs2005

vs2015:
	premake5 vs2015
		
-include $(ROOT)/clean.mk	

	

