ROOT=../../..
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC12101 board 

ifeq ($(OS),Windows_NT)
SHELL 			 = cmd
endif

-include nmc4vars_win.mk

.SUFFIXES:

ifndef MC12101
$(warning *** WARNING: 'MC12101' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

CC       = nmc-g++
AS       = nmc-gcc
CC_FLAGS = -std=c++11 -O2 -Wall -mnmc4-float
PROJECT  = main
TARGET   =$(PROJECT)
INC_DIRS = -I$(ROOT)/include
LIB_DIRS = -L"$(NMCGCC)/lib" -L"$(NMCGCC)/nmc/lib/nmc4"  -L"$(ROOT)/lib"
LIBS     = -lhal-mc12101

CFG      =  "-Wl,-cmc12101brd-nmpu0.cfg"
SRC_CPP  = $(wildcard ../*.cpp)

$(TARGET): $(SRC_CPP) Makefile
	$(CC) $(CC_FLAGS) $(SRC_CPP) $(INC_DIRS) $(LIB_DIRS) $(LIBS) -lc $(CFG) -Wl,--whole-archive -lmc12101 -Wl,--no-whole-archive -o $(TARGET)

PATH:=$(MC12101)/bin;$(PATH)

run: $(TARGET)
	mc12101run $(TARGET)


nmc4vars_win.mk:
	copy "$(NMC_GCC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk" nmc4vars_win.mk
	
rund:
	-$(MAKE) run
	pause

vs2005:
	premake5 vs2005

vs2015:
	premake5 vs2015
	
	
clean:
	-rm -f *.abs *.elf .*  *.o *.db *.user nmc4vars_win.mk
	-rm $(TARGET) 
	-rm -d -r bin obj


	