ROOT = ../../..
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC12101 board 

-include nmc4vars_win.mk
-include $(ROOT)/clean.mk
HAL = $(ROOT)
.SUFFIXES:


ifndef MC12101
$(warning *** WARNING: 'MC12101' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

PROJECT  = main
TARGET   = $(PROJECT)

CC       = nmc-g++
AS       = nmc-gcc
CXXFLAGS = -std=c++11 -O2 -Wall -mnmc4-float
LIBS     = -lhal-mc12101 -lc  -Wl,--whole-archive -lmc12101 -Wl,--no-whole-archive
MAP      = $(PROJECT).map
CFG      = mc12101brd-nmpu0.cfg
LDFLAGS  = -Wl,-Map=$(MAP) -Wl,-c$(CFG)

INC_DIRS = -I"$(HAL)/include"
LIB_DIRS = -L"$(NMCGCC)/lib" -L"$(NMCGCC)/nmc/lib/nmc4" -L"$(HAL)/lib"
SRC_DIRS = ../src_nm0
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,$(SRC_DIRS)))
SRC_C    = $(wildcard $(addsuffix /*.c,$(SRC_DIRS)))
SRC_ALL  = $(SRC_CPP) $(SRC_ASM) $(SRC_C)

.DEFAULT_GOAL := $(TARGET)

$(TARGET): $(SRC_ALL) Makefile
	$(CC) -o$(TARGET) $(CXXFLAGS) $(LDFLAGS) $(SRC_ALL) $(INC_DIRS) $(LIB_DIRS) $(LIBS)

PATH:=$(MC12101)/bin;$(PATH)

run: 	$(TARGET)
	mc12101run $(TARGET)

reset:
	mc12101reset
	
nmc4vars_win.mk:
	copy "$(NMC_GCC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk" nmc4vars_win.mk
	
vs2015:
	premake5 vs2015
	
vs2008:
	premake5 vs2008		