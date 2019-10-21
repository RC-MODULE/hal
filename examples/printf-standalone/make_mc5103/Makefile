ROOT = ../../..
include $(ROOT)/global.mk

.SUFFIXES:

BOARD    = mc5103


INC_DIRS = -I$(ROOT)/include
LIB_DIRS = -L$(ROOT)/lib 
PROJECT  = main1
TARGET   = $(PROJECT).abs
MAP      = $(PROJECT).map
CFG      = mc5103brd.cfg
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,../src_nm0))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,../src_nm0))
SRC_ALL  = $(SRC_CPP) $(SRC_ASM)



PATH:=$(MC5103)/bin;$(PATH)

TARGET   = main.abs
CFG      = mc5103brd.cfg
INC_DIRS = -I"$(MC5103)/include" -I$(ROOT)/include
LIB_DIRS = -L"$(MC5103)/lib"     -L$(ROOT)/lib
LIBS     = 
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,../src_nm0))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,../src_nm0))
HEAP     = -heap=14384 -heap1=16384 -heap2=16384 -heap3=16384


ifdef NMCC
#################### OLD NMSDK ##################
ifndef MC5103
$(error ERROR: 'MC5103' environment variable is not defined!  )
endif


CC	 = nmcc 
LIBS     = mc5103lib.lib hal-mc5103.lib libint05.lib
HEAP     = -heap=6384 -heap1=6384 -heap2=16384 -heap3=16384
CC_FLAGS = -nmc3 -g -O0
LD_FLAGS = -c$(CFG) $(HEAP) -m$(MAP)
INC_DIRS+= -I"$(MC12101)/include" -I"$(NEURO)/include"
LIB_DIRS+= -L"$(MC12101)/lib"  
else 
#################### GCC SDK ####################
-include nmc4vars_win.mk
CC       = nmc-g++
AS       = nmc-gcc
#-lhal-mc5103 
LIBS     =  -lc -Wl,--whole-archive -lmc5103lib -Wl,--no-whole-archive
CC_FLAGS = -std=c++11 -O2 -Wall -mnmc3
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




PATH:=$(MC5103)/bin;$(PATH)



run: 
	"$(MC5103)/bin/mc5103run" $(TARGET) 

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


	