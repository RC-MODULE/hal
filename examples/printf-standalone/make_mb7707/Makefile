ROOT = ../../..
include $(ROOT)/global.mk

.SUFFIXES:

BOARD    = mb7707


INC_DIRS = -I$(ROOT)/include
LIB_DIRS = -L$(ROOT)/lib 
PROJECT  = main
TARGET   = $(PROJECT).abs
MAP      = $(PROJECT).map
CFG      = mb7707brd.cfg
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,../src_nm0))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,../src_nm0))
SRC_ALL  = $(SRC_CPP) $(SRC_ASM)



ifdef NMCC
#################### OLD NMSDK ##################
ifndef MB7707
$(warning*** WARNING: 'MB7707' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

ifndef MB7707MAC
$(warning*** WARNING: 'MB7707MAC' environment variable is not defined! Board SDK might not have installed correctly. )
endif 


CC	 = nmcc 
LIBS     =  mb7707lib.lib hal-mb7707.lib libint_soc.lib
HEAP     = -heap=6384 -heap1=6384 -heap2=16384 -heap3=16384
CC_FLAGS = -soc -g -O0
LD_FLAGS = -c$(CFG) $(HEAP) -m$(MAP)
INC_DIRS+= -I"$(MB7707)/include" -I"$(NEURO)/include"
LIB_DIRS+= -L"$(MB7707)/lib"  
else 
#################### GCC SDK ####################
-include nmc4vars_win.mk
CC       = nmc-g++
AS       = nmc-gcc
#-lhal-mc5103
LIBS     =  -lc -Wl,--whole-archive -lmb7707lib -Wl,--no-whole-archive
#CC_FLAGS = -std=c++11 -O3 -Wall -mnmc3
CC_FLAGS =  -mnmc3
LD_FLAGS = -Wl,-Map=$(MAP) -Wl,-c$(CFG)
INC_DIRS+= 
LIB_DIRS+=  -L "$(NMC_GCC_TOOLPATH)/nmc4-ide/lib" -L"$(MB7707)/lib"

endif
#################################################

all: $(TARGET) 

$(TARGET): $(SRC_CPP) $(SRC_ASM) Makefile $(CFG)
	$(CC) -o$(TARGET) $(SRC_ALL) $(INC_DIRS) $(LIB_DIRS) $(LIBS)  $(CC_FLAGS) $(LD_FLAGS)
	

nmc4vars_win.mk:
	copy "$(NMC_GCC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk" nmc4vars_win.mk




PATH:=$(MB7707)/bin;$(PATH)



run: 
	"$(MB7707)/bin/mb7707run" -a$(MB7707MAC) -i $(TARGET) 

rund:
	-$(MAKE) run
	pause

clean:
	-$(OS_RM) *.abs *.elf *.ncb *.map *.elf *.suo *.user *.filters
	-$(OS_RD) bin obj
	
kill: clean
	-$(OS_RM) *.vcproj *.sln *.vcxproj 
	
vs2005:	
	premake5 vs2005
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(HOST).vcproj
	
vs2015:	
	premake5 vs2015
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(HOST).vcxproj


	

