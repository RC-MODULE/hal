ROOT = ..
-include $(ROOT)/global.mk
.SUFFIXES:

DEBUG = y;

PROJECT  = libhal-mc12101
CONFIGURATION ?= Release
ifdef DEBUG
CONFIGURATION =Debug
else
$(error Release configuration cause bug in printf. Run with flag DEBUG=y )
endif


INC_DIRS = -I$(ROOT)/include -I../src_1879vm6ya
LIB_DIRS = 
SRC_DIRS = ../src_1879vm6ya ../src_nm_io ../src_ringbuffer
ALL_CPP  = $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
ALL_C    = $(wildcard $(addsuffix /*.c  ,$(SRC_DIRS)))
ALL_ASM  = $(wildcard $(addsuffix /*.asm,$(SRC_DIRS)))
ALL_S    = $(wildcard $(addsuffix /*.s,  $(SRC_DIRS)))
OUT_DIR  = $(ROOT)/lib
#--------------  RELEASE/ALL config -------------------
TARGET           = $(OUT_DIR)/$(PROJECT).a
ARCH             =nmc4
AS               =asm 				 
AS_FLAGS         =-$(ARCH) -nm2ms  $(INC_DIRS) -split_sir -W-111 -W-109
AS_FLAGS_C2ASM   =-$(ARCH) -nm2ms  $(INC_DIRS) -split_sir -W-111 -W-109
LIBS             =
CC               =nmc-g++
CCPP_FLAGS       =-Wall $(INC_DIRS)  -std=c++11 
CC_FLAGS         =-Wall $(INC_DIRS)  -std=c++11
BUILDER          =nmc-ar
BUILDER_FLAGS    =rcs $(TARGET)
TMP_DIR          =$(CONFIGURATION)
#--------------  DEBUG config -------------------------
ifdef DEBUG
TARGET           =$(OUT_DIR)/$(PROJECT).a
CCPP_FLAGS       =-Wall -std=c++11 $(INC_DIRS)
CC_FLAGS         =-Wall -std=c++11 $(INC_DIRS)
AS_FLAGS        +=-g
#BUILDER_FLAGS   +=rcs $(TARGET)
endif 

# collecting of objects
OBJECTS_CPP = $(notdir $(patsubst %.cpp,%.o,$(ALL_CPP)))
OBJECTS_ASM = $(notdir $(patsubst %.asm,%.o,$(ALL_ASM)))
OBJECTS_S   = $(notdir $(patsubst %.s,%.o,$(ALL_S)))
OBJECTS_C   = $(notdir $(patsubst %.c,%.o,$(ALL_C)))
OBJECTS     = $(addprefix $(TMP_DIR)/,$(OBJECTS_C) $(OBJECTS_CPP)  $(OBJECTS_ASM) $(OBJECTS_S))

VPATH    = $(SRC_DIRS)

-include 		 nmc4vars_win.mk

#========================== NeuroMatrix build ===================	
nmc: echo $(TARGET)

echo:
	$(info *******************************************************************************)
	$(info **                                                                           **)
	$(info **                       Neuro Matrix <$(CONFIGURATION)> compiling...          )
	$(info **                                                                           **)
	$(info *******************************************************************************)

default: 
	$(MAKE) $(TARGET) -j4 

$(TARGET): $(TMP_DIR) $(OBJECTS) nmc4vars_win.mk
	$(BUILDER) $(BUILDER_FLAGS) $(OBJECTS)
	@echo *******************************************************************************
	@echo **                                                                           **
	@echo **                                   Build OK!                               **
	@echo **                                                                           **
	@echo *******************************************************************************

nmc4vars_win.mk:
	copy "$(NMC_GCC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk" nmc4vars_win.mk

$(TMP_DIR):
	-mkdir "$(@)"

$(OUT_DIR): 
	-mkdir "$(@)"
	
$(TMP_DIR)/%.o: %.asm 
	$(AS) $(AS_FLAGS) $(<) -o$(@)
	
$(TMP_DIR)/%.o: %.cpp 
	$(CC) $(CCPP_FLAGS) -c $(<) -o$(@) $(INC_DIRS) 

$(TMP_DIR)/%.o: %.c
	$(CC) $(CC_FLAGS) -c $(<) -o$(@) $(INC_DIRS) 

#========================== Visual Studio build ===================	

	
vs2005: $(PROJECT).sln
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).sln  "$(CONFIGURATION)|Win32"
	

vs2015:	$(PROJECT).sln
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).sln /t:Build /p:Configuration=$(CONFIGURATION)

	
$(PROJECT).sln:	premake5.lua 
	premake5 $(MAKECMDGOALS) 
	
#--------------- cleanup  --------------------------------	


-include $(ROOT)/clean.mk