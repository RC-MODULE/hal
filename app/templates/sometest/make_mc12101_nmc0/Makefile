


###########################################################################################
# (RC)Module 7/8/2015
#
# This is a universal Makefile for NeuroMatrix projects (application or library) 
# with automatic search of source files , compiling and linking.
# Makefile is processed by GNU make and may be used both in Linux and Windows OS
# Under OS Windows you need:
# * GNU Make           http://gnuwin32.sourceforge.net/packages/make.htm
#
# USAGE:
#   For release build                    run 'make'
#   For debug   build                    run 'make DEBUG=y'
#   For debug & release  build           run 'make all'
#   For full  rebuild                    run 'make -B'
#   Multithread build (speed up)         run 'make -j4'
#   To clean                             run 'make clean'
#   To create VisualStudio 2005 project  run 'make vs2005'
#   To create VisualStudio 2013 project  run 'make vs2015'
#   Execute target                       run 'make run'
#   Execute debug target                 run 'make rund'
#
# WARNING:
#   Makefile is sensitive to leading or trailing spaces and backslashes in the pathes
#
# DEFENITIONS:
#   PROJECT         - project name     
#   TARGET          - output target (.lib or .abs) file with path
#   INC_DIRS        - set of search pathes for the macro libraries and includes (with -I flag separated)
#   SRC_DIRS        - set of explicit  search pathes of all (.c .cpp .asm) source files (with space separated)
#   LIB_DIRS        - set of search library pathes (with -l flag separated)
#   TMP_DIR         - temporary build-folder (Debug/Release)
#   LIBS            - set of (.lib) libraries to be linked (with space separated)
#   
#   CC              - C/C++ compiler(nmcpp/nmcc)
#   CC_FLAGS        - C/C++ compiler(nmcpp/nmcc) flags   
#   AS				- assembler(asm/nmcc)
#   AS_FLAGS        - assembler(asm/nmcc) flags for compiling of pure .asm files 
#   AS_FLAGS_C2ASM  - assembler(asm/nmcc) flags for compiling of .asm files compiled from .c/.cpp sources
#   BUILDER         - builder  (libr/linker) 			
#   BUILDER_FLAGS   - build flags of (libr/linker)
#
#   HEADER_DIRS     - set of pathes for .h   files to be included into Visual Studio project (with space separated)
#   RESOURCES       - set of any additionals files to be included into Visual Studio project
###############################################################################################

#============== PROJECT & BUILD CONFIGURATION ==========
#
include $(ROOT)/global.mk
#--------------  target & input dirs -------------------
PROJECT          = test_mc12101
ifndef MC12101
$(warning *** WARNING: 'MC12101' environment variable is not defined! Board SDK might not have installed correctly. )
endif

OUT_DIR          = .
TARGET           = $(OUT_DIR)/$(PROJECT).abs
INC_DIRS         = -I"$(NEURO)/include" -I"$(MC12101)/include" -I"$(NMPP)/include"  -I"$(NMPP)/include/nmpli"  -I$(HAL)/include -I$(ROOT)/include
SRC_DIRS         = $(ROOT)/src/nm $(ROOT)/src/common ..
LIB_DIRS         = -l"$(NEURO)/lib"   -l"$(MC12101)/lib" -l"$(NMPP)/lib" -l$(HAL)/lib 
TMP_DIR          = Release
#--------------  RELEASE/ALL config -------------------
HEAPS 			?= -heap=118384 -heap1=18384 -heap2=18384 -heap3=25000
AS               = asm 				 
AS_FLAGS         = -nmc4 -nm2ms  $(INC_DIRS) -split_sir
AS_FLAGS_C2ASM   = -nmc4 -nm2ms  $(INC_DIRS) -split_sir
LIBS             = mc12101lib_nm.lib libc4.lib libc4f.lib nmpp-nmc4f.lib hal-mc12101.lib cppnew4.lib libint_6407.lib
CC               = nmcpp
CC_FLAGS         = -6405 -DNEURO -OPT2 -inline 
BUILDER          = linker
BUILDER_FLAGS    = -cmc12101brd-nmc0.cfg -m  $(HEAPS) -full_names -o$(TARGET) $(LIB_DIRS)
#--------------  DEBUG config -------------------------
ifdef DEBUG      
TARGET           = $(OUT_DIR)/$(PROJECT)d.abs
CC_FLAGS         = -6405 -DNEURO -OPT0 -inline -debug 
#LIBS            := $(subst nmc3.lib,nmc3d.lib,$(LIBS))
AS_FLAGS        += -ga
BUILDER_FLAGS   += -d0
TMP_DIR          = Debug
endif 


#=================== SOURCE & OBJECTS COLLECTION ===========================

.SUFFIXES:

# regular search of *.c *.cpp,*.asm sources in all $(SRC_DIRS)
ALL_CPP =$(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
ALL_ASM =$(wildcard $(addsuffix /*.asm,$(SRC_DIRS)))
ALL_C   =$(wildcard $(addsuffix /*.c,$(SRC_DIRS)))
ALL_H   =$(wildcard $(addsuffix /*.h,$(HEADER_DIRS)))

VPATH    = $(SRC_DIRS)

# collecting of objects
OBJECTS_CPP = $(notdir $(patsubst %.cpp,%.o,$(ALL_CPP)))
OBJECTS_ASM = $(notdir $(patsubst %.asm,%.o,$(ALL_ASM)))
OBJECTS_C   = $(notdir $(patsubst %.c,%.o,$(ALL_C)))
OBJECTS     = $(addprefix $(TMP_DIR)/,$(OBJECTS_C) $(OBJECTS_CPP)  $(OBJECTS_ASM))

#======================== BUILD RULES ====================================
.PHONY:all  clean skip

.DEFAULT_GOAL=default
default: $(TMP_DIR) $(OUT_DIR)  $(TARGET)

all: 
	$(MAKE) -j4

	
$(TMP_DIR):
	-mkdir "$(@)"

$(TARGET): $(TMP_DIR) $(OBJECTS) Makefile
	$(BUILDER) $(BUILDER_FLAGS) $(OBJECTS) $(LIBS)

$(OUT_DIR): 
	-mkdir "$(@)"
	
$(TMP_DIR)/%.o: %.asm 
	$(AS) $(AS_FLAGS) $(<) -o$(@)
	
$(TMP_DIR)/%.o: $(TMP_DIR)/%.asmx 
	$(AS) $(AS_FLAGS_C2ASM) $(<) -o$(@)

$(TMP_DIR)/%.asmx: %.cpp 
	$(CC) $(CC_FLAGS) $(<) -O$(@) $(INC_DIRS) 

$(TMP_DIR)/%.asmx: %.c
	$(CC) $(CC_FLAGS) $(<) -O$(@) $(INC_DIRS) 

#========================== Visual Studio  ===================
export ALL_CPP ALL_ASM ALL_C ALL_H

$(PROJECT).vcxproj :	premake5.lua 
	premake5 vs2015

$(PROJECT).vcproj :	premake5.lua 
	premake5 vs2005

vs2005:    	$(PROJECT).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).vcproj
#	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).vcproj /p:Configuration=Release
	
vs2015:    	$(PROJECT).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Release
#========================= test & run ===================
run: $(TARGET)
	mc12101run $(TARGET)

test: .exitcode
	
ifeq ($(OS),Windows_NT)	
.exitcode: $(TARGET) .test.bat
	.test.bat 
	
.test.bat: 
	@echo mc12101run -p -v $(TARGET) >.test.bat
	@echo @echo %%errorlevel%% ^>.exitcode >>.test.bat
else 
.exitcode: $(TARGET)
	mc12101run ./$(TARGET) ; echo $$? > .exitcode
endif

include $(ROOT)/clean.mk