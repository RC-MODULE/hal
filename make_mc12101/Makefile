ROOT = ..
-include $(ROOT)/global.mk



.SUFFIXES:
PROJECT   	 = hal-mc12101
PROJECT_X86  = $(PROJECT)-x86


TARGET_X86 = $(OUT_DIR)/$(PROJECT_X86).lib

OUT_DIR  = ../lib
INC_DIRS = -I$(ROOT)/include -I"$(NEURO)/include"  -I../src_1879vm6ya
LIB_DIRS = 
SRC_DIRS = ../src_1879vm6ya ../src_mc12101  ../src_nmc_all ../src_nmc_all
ALL_CPP  = $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
ALL_C    = $(wildcard $(addsuffix /*.c  ,$(SRC_DIRS)))
ALL_ASM  = $(wildcard $(addsuffix /*.asm,$(SRC_DIRS)))
ALL_S    = $(wildcard $(addsuffix /*.s,  $(SRC_DIRS)))
OUT_DIR  = $(ROOT)/lib
#--------------  RELEASE/ALL config -------------------
TARGET           = $(OUT_DIR)/$(PROJECT).lib
ARCH             =nmc4
AS               =asm 				 
AS_FLAGS         =-$(ARCH) -nm2ms  $(INC_DIRS) -split_sir -W-111 -W-109
AS_FLAGS_C2ASM   =-$(ARCH) -nm2ms  $(INC_DIRS) -split_sir -W-111 -W-109
LIBS             =
CC               =nmcpp
CCPP_FLAGS       =-nmc3 -DNEURO -OPT0 -inline 
CC_FLAGS         =$(CCPP_FLAGS) -except -rtti
BUILDER          =libr
BUILDER_FLAGS    =-s $(TARGET)
TMP_DIR          =Release
#--------------  DEBUG config -------------------------
ifdef DEBUG      
TARGET           =$(OUT_DIR)/$(PROJECT).lib
CCPP_FLAGS       =-nmc3 -DNEURO -OPT0 -inline -debug 
CC_FLAGS         =$(CCPP_FLAGS) -except -rtti
LIBS            :=
AS_FLAGS        +=-ga
BUILDER_FLAGS   +=-d0
TMP_DIR          =Debug
endif 

# collecting of objects
OBJECTS_CPP = $(notdir $(patsubst %.cpp,%.o,$(ALL_CPP)))
OBJECTS_ASM = $(notdir $(patsubst %.asm,%.o,$(ALL_ASM)))
OBJECTS_S   = $(notdir $(patsubst %.s,%.o,$(ALL_S)))
OBJECTS_C   = $(notdir $(patsubst %.c,%.o,$(ALL_C)))
OBJECTS     = $(addprefix $(TMP_DIR)/,$(OBJECTS_C) $(OBJECTS_CPP)  $(OBJECTS_ASM) $(OBJECTS_S))

VPATH    = $(SRC_DIRS)


#all: nmc x86

#--------------- nmc build --------------------------------	
nmc: echo $(TARGET)

echo 
	$(info *******************************************************************************)
	$(info **                                                                           **)
	$(info **                       Neuro Matrix  compiling...                          **)
	$(info **                                                                           **)
	$(info *******************************************************************************)

$(TARGET): $(TMP_DIR) $(OBJECTS) 
	$(BUILDER) $(BUILDER_FLAGS) $(OBJECTS) 

$(TMP_DIR):
	-mkdir "$(@)"

$(OUT_DIR): 
	-mkdir "$(@)"
	
$(TMP_DIR)/%.o: %.asm 
	$(AS) $(AS_FLAGS) $(<) -o$(@)
	
$(TMP_DIR)/%.o: $(TMP_DIR)/%.asmx 
	$(AS) $(AS_FLAGS_C2ASM) $(<) -o$(@)

$(TMP_DIR)/%.asmx: %.cpp 
	$(CC) $(CCPP_FLAGS) $(<) -O$(@) $(INC_DIRS) 

$(TMP_DIR)/%.asmx: %.c
	$(CC) $(CC_FLAGS) $(<) -O$(@) $(INC_DIRS) 

#--------------- X86 build --------------------------------	
x86: $(VS_VER)

	
vs2005: $(PROJECT).sln
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).sln 

vs2015:	$(PROJECT).sln
	"$(VS140COMNTOOLS)vsvars32" && msbuild.exe $(PROJECT).sln /t:Build
	
$(PROJECT).sln:	premake5.lua 
	premake5 $(MAKECMDGOALS) 
	
#--------------- cleanup  --------------------------------	


-include $(ROOT)/clean.mk