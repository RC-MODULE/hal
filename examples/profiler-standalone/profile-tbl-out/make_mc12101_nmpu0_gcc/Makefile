ROOT=../../../..

# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC12101 board 


-include nmc4vars_win.mk

.SUFFIXES:

ifndef MC12101
$(warning *** WARNING: 'MC12101' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

PROJECT  = main
TARGET   = $(PROJECT)
CC       = nmc-g++
AS       = nmc-gcc
CC_FLAGS = -std=c++11 -O1 -Wall -mnmc4-float  -mmas
PROJECT  = main
TARGET   =$(PROJECT)
INC_DIRS = -I$(ROOT)/include
LIB_DIRS = -L"$(NMCGCC)/lib" -L"$(NMCGCC)/nmc/lib/nmc4"  -L"$(ROOT)/lib"
LIBS     = -lhal-mc12101
MAP      = $(PROJECT).map
CFG      =  mc12101brd-nmpu0.cfg
SRC_DIRS = .. ./
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,$(SRC_DIRS)))


$(TARGET): $(SRC_CPP) Makefile
	$(CC)  $(SRC_CPP) $(SRC_ASM) $(CC_FLAGS) $(INC_DIRS) $(LIB_DIRS) $(LIBS) -lc -Wl,-c$(CFG) -Wl,--whole-archive -lmc12101 -Wl,--no-whole-archive -fno-threadsafe-statics  -fno-exceptions -o $(TARGET) -Wl,-Map=$(MAP) 
	
#	2>&1|errecho

PATH:=$(MC12101)/bin;$(PATH)



nmc4vars_win.mk:
	copy "$(NMC_GCC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk" nmc4vars_win.mk
	
SRC_DIRS += .

-include $(ROOT)/global.mk
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC5103 board 



ifeq ($(OS),Windows_NT)
  SHELL = cmd
endif

run: $(TARGET)
	mc12101run $(TARGET)
	
rund:
	-$(MAKE) run
	pause

vs2005:
	premake5 vs2005

vs2015:
	premake5 vs2015

erase:
	mc12101erase
	
profile:	
	python $(ROOT)/script/profiler.py > proflist_generated.asm
	$(MAKE)
	python $(ROOT)/script/profiler-gcc.py $(MAP) > proflist_generated.asm
	
	
-include $(ROOT)/clean.mk	

	