ROOT = ..
-include $(ROOT)/global.mk
.SUFFIXES:



PROJECT  = hal-mc5103
CONFIGURATION ?= Release
ifdef DEBUG
CONFIGURATION =Debug
else

endif


INC_DIRS = -I$(ROOT)/include -I"$(NEURO)/include"  -I"$(MC5103)/include" -I"../src_1879vm5ya" -I$(NMPP)/include
LIB_DIRS = 
SRC_DIRS = ../src_1879vm5ya  ../src_nm_io ../src_nmc_all ../src_printf_nmcpp ../src_ringbuffer
ALL_CPP  = $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
ALL_C    = $(wildcard $(addsuffix /*.c  ,$(SRC_DIRS)))
ALL_ASM  = $(wildcard $(addsuffix /*.asm,$(SRC_DIRS)))
ALL_S    = $(wildcard $(addsuffix /*.s,  $(SRC_DIRS)))
OUT_DIR  = $(ROOT)/lib
#--------------  RELEASE/ALL config -------------------
TARGET           = $(OUT_DIR)/$(PROJECT).lib
ARCH             =nmc3
AS               =asm 				 
AS_FLAGS         =-$(ARCH) -nm2ms  $(INC_DIRS) -split_sir -W-111 -W-109
AS_FLAGS_C2ASM   =-$(ARCH) -nm2ms  $(INC_DIRS) -split_sir -W-111 -W-109
LIBS             =
CC               =nmcpp
CCPP_FLAGS       =-nmc3 -DNEURO -OPT2 -inline 
CC_FLAGS         =$(CCPP_FLAGS) -except -rtti
BUILDER          =libr
BUILDER_FLAGS    =-s $(TARGET)
TMP_DIR          =$(CONFIGURATION)
#--------------  DEBUG config -------------------------
ifdef DEBUG      
TARGET           =$(OUT_DIR)/$(PROJECT).lib
CCPP_FLAGS       =-nmc3 -DNEURO -OPT0 -inline -debug 
CC_FLAGS         =$(CCPP_FLAGS) -except -rtti
AS_FLAGS        +=-ga
BUILDER_FLAGS   +=-d0 -full_names
endif 

# collecting of objects
OBJECTS_CPP = $(notdir $(patsubst %.cpp,%.o,$(ALL_CPP)))
OBJECTS_ASM = $(notdir $(patsubst %.asm,%.o,$(ALL_ASM)))
OBJECTS_S   = $(notdir $(patsubst %.s,%.o,$(ALL_S)))
OBJECTS_C   = $(notdir $(patsubst %.c,%.o,$(ALL_C)))
OBJECTS     = $(addprefix $(TMP_DIR)/,$(OBJECTS_C) $(OBJECTS_CPP)  $(OBJECTS_ASM) $(OBJECTS_S))

VPATH    = $(SRC_DIRS)


#========================== NeuroMatrix build ===================	
.DEFAULT_GOAL:=legacy

logo:
	$(info *******************************************************************************)
	$(info **                                                                           **)
	$(info **                       Neuro Matrix <$(CONFIGURATION)> legacy compiling...   )
	$(info **                                                                           **)
	$(info *******************************************************************************)

legacy: logo $(OUT_DIR) $(TMP_DIR) $(OBJECTS) 
	$(BUILDER) $(BUILDER_FLAGS) $(OBJECTS) 
	@echo *******************************************************************************
	@echo **                                                                           **
	@echo **                                   Build OK!                               **
	@echo **                                                                           **
	@echo *******************************************************************************

# custom build of printf.c because of error with -O2 optimization compiling
$(TMP_DIR)/printf.o: ../src_printf_nmcpp/printf.c
	nmcc ../src_printf_nmcpp/printf.c -Sc -nmc3 -o$(TMP_DIR)/printf.o -O0 $(INC_DIRS)

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

