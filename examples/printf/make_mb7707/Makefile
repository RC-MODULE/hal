ROOT = ../../..
HAL  = $(ROOT)
include $(ROOT)/global.mk
#-include ../local.mk

# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC5103 board and stores return code in ".exitcode"  file

.SUFFIXES:


ifndef MB7707
$(error ERROR: 'MB7707' environment variable is not defined!  )
endif 


BOARD    = mb7707
HOST     = $(BOARD)-x86
PROJECT  = mb7707-printf
CONFIGURATION = Debug

TARGET   = main0.abs
INC_DIRS = -I"$(MB7707)/include" -I$(ROOT)/include -I"$(NEURO)/include"
LIB_DIRS = -L"$(MB7707)/lib" -L$(HAL)/lib -L$(NMPP)/lib
LIBS     = hal-mb7707.lib mb7707lib.lib 
CFG      = mb7707brd.cfg
SRC_DIRS = ../src_nm0
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,$(SRC_DIRS)))
HEAPS    = -heap=1658800 -heap1=32500 -heap2=32500 -heap3=32500 


ALL: $(TARGET) 

$(TARGET): $(SRC_CPP) $(SRC_ASM) $(CFG) Makefile
	nmcc -o$(TARGET) -m.main.map $(SRC_CPP) $(SRC_ASM)  -nmc3 -g -O0 $(INC_DIRS) $(LIB_DIRS) $(LIBS) -c$(CFG) -heap=6384 -heap1=6384 -heap2=6384 -heap3=6384


PATH:=$(MB7707)/bin;$(PATH)


	
vs2005: $(PROJECT).sln
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).sln  "$(CONFIGURATION)|Win32"
	

vs2015:	$(PROJECT).sln
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).sln /t:Build /p:Configuration=$(CONFIGURATION)

	
$(PROJECT).sln:	premake5.lua 
	premake5 $(MAKECMDGOALS) 

run: 
	bin\debug\$(HOST)
	
mbrun mcrun: $(TARGET) 
	mb7707run -i $(TARGET)

include $(ROOT)/clean.mk


	