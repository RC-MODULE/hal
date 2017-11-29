ROOT = ..
-include $(ROOT)/global.mk


.SUFFIXES:
PROJECT  = hal-mb7707
LIB_X86   = $(PROJECT)-x86
LIBIO_X86 = $(PROJECT)io-x86
INC_DIRS = -I$(ROOT)/include -I"$(NEURO)/include"  -I"$(MB7707)/libload"
LIB_DIRS = 
SRC_DIRS = ../src_1879hb1ya ../src_mb7707  ../src_nmc_all
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
CC_FLAGS         =$(CCPP_FLAGS) -Tc99
BUILDER          =libr
BUILDER_FLAGS    =-s $(TARGET)
TMP_DIR          =Release
#--------------  DEBUG config -------------------------
ifdef DEBUG      
TARGET           =$(OUT_DIR)/$(PROJECT).lib
CCPP_FLAGS       =-nmc3 -DNEURO -OPT0 -inline -debug 
CC_FLAGS         =$(CCPP_FLAGS) -Tc99
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


all: $(TARGET) $(OUT_DIR)/$(LIB_X86).lib $(OUT_DIR)/$(LIBIO_X86).lib

#--------------- nmc build --------------------------------	
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
	
$(OUT_DIR)/$(LIB_X86).lib: $(LIB_X86).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(LIB_X86).vcproj

$(OUT_DIR)/$(LIBIO_X86).lib: $(LIBIO_X86).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(LIBIO_X86).vcproj
	
$(LIB_X86).vcproj $(LIBIO_X86).vcproj  vs2005:	premake5.lua 
	premake5 vs2005
	
#--------------- cleanup  --------------------------------	

clean:
	-$(OS_RM) *.abs *.elf *.ncb *.map *.elf *.suo *.user *.filters $(OUT_DIR)/$(LIB_X86).lib $(OUT_DIR)/$(LIBIO_X86).lib $(OUT_DIR)/$(PROJECT).lib $(OUT_DIR)/$(PROJECT)d.lib
	-$(OS_RD) bin obj Release Debug
	
kill: clean
	-$(OS_RM) *.vcproj *.sln *.vcxproj
	
	