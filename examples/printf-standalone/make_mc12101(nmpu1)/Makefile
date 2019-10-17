ROOT = ../../..
include $(ROOT)/global.mk

.SUFFIXES:

BOARD    = mc12101
HOST     = $(BOARD)-x86

INC_DIRS = -I$(ROOT)/include
LIB_DIRS = -L$(ROOT)/lib 
PROJECT  = main1
TARGET   = $(PROJECT).abs
MAP      = $(PROJECT).map
CFG      = mc12101brd-nmpu1.cfg
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,../src_nm1))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,../src_nm1))
SRC_ALL  = $(SRC_CPP) $(SRC_ASM)


ifdef NMCC
#################### OLD NMSDK ##################
ifndef MC12101
$(warning *** WARNING: 'MC12101' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

CC	 = nmcc 
LIBS     = mc12101lib_nm_old.lib libc4.lib  hal-mc12101.lib
HEAP     = -heap=6384 -heap1=6384 -heap2=16384 -heap3=16384
CC_FLAGS = -nmc4 -g -O0
LD_FLAGS = -c$(CFG) $(HEAP) -m$(MAP)
INC_DIRS+= -I"$(MC12101)/include" -I"$(NEURO)/include"
LIB_DIRS+= -L"$(MC12101)/lib"  
else 
#################### GCC SDK ####################
-include nmc4vars_win.mk
CC       = nmc-g++
AS       = nmc-gcc
LIBS     =  -lhal-mc12101 -lc -Wl,--whole-archive -lmc12101 -Wl,--no-whole-archive
CC_FLAGS = -std=c++11 -O2 -Wall -mnmc4
LD_FLAGS = -Wl,-Map=$(MAP) -Wl,-c$(CFG)
INC_DIRS+= 
LIB_DIRS+=  -L "$(NMC_GCC_TOOLPATH)/nmc4-ide/lib"

endif
#################################################

all: $(TARGET) 

$(TARGET): $(SRC_CPP) $(SRC_ASM) Makefile $(CFG)
	$(CC) -o$(TARGET) $(SRC_ALL) $(INC_DIRS) $(LIB_DIRS) $(LIBS)  $(CC_FLAGS) $(LD_FLAGS)
	

nmc4vars_win.mk:
	copy "$(NMC_GCC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk" nmc4vars_win.mk




PATH:=$(MC12101)/bin;$(PATH)



run: 
	mc12101run $(TARGET) -p


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
	