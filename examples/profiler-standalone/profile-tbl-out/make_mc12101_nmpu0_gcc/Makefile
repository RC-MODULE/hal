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
#   To create VisualStudio 2005 project  run 'make vs8'
#   To create VisualStudio 2013 project  run 'make vs13'
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
#   SRC_DIRS        - set of explicit search pathes of all (.c .cpp .asm) source files (with space separated)
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

ROOT             = ../../../..
include $(ROOT)/global.mk
include 		 nmc4vars_win.mk
HAL = $(ROOT)
#-------------- target & input dirs -------------------
PROJECT          = main0
OUT_DIR          = .
TARGET           = $(OUT_DIR)/$(PROJECT).abs
INC_DIRS         = -I$(HAL)/include -Ig:/hal/include
SRC_DIRS         = .. .
LIB_DIRS         = -L"$(NMC_GCC_TOOLPATH)/nmc4-ide/lib" -L"$(NMPP)/lib" -L"$(HAL)/lib"
TMP_DIR          = Release
#-------------- RELEASE/ALL config -------------------
CC 				 = nmc-g++
#AS               = nmc-as-gnu 				 
#AS_FLAGS    	 = -nmc4  --traditional-format
#AS               = nmc-gcc				 
#AS_FLAGS    	 = -c -mmas -x assembler -mnmc4-float 
AS               = asm 				 
AS_FLAGS    	 = -nmc4
LIBS             = -lnmpp-nmc4f -lhal-mc12101
LINKER_FLAGS     =   -Xlinker -Map=.main0.map $(LIB_DIRS) -save-temps "-Wl,-cmc12101brd-nmpu0.cfg" -Wl,--whole-archive -lmc12101 -Wl,--no-whole-archive -o$(TARGET)

#=================== SOURCE & OBJECTS COLLECTION ===========================

.SUFFIXES:

# regular search of *.c *.cpp,*.asm sources in all $(SRC_DIRS)
ALL_CPP =$(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
ALL_ASM =$(wildcard $(addsuffix /*.asm, $(SRC_DIRS))) 
ALL_S   =$(wildcard $(addsuffix /*.s, $(SRC_DIRS)))
ALL_C   =$(wildcard $(addsuffix /*.c, $(SRC_DIRS)))
ALL_H   =$(wildcard $(addsuffix /*.h, $(HEADER_DIRS)))

VPATH   = $(SRC_DIRS)

# collecting of objects
OBJECTS_CPP = $(notdir $(patsubst %.cpp,%.o,$(ALL_CPP)))
OBJECTS_ASM = $(notdir $(patsubst %.asm,%.o,$(ALL_ASM))) 
OBJECTS_S   = $(notdir $(patsubst %.s,%.o,$(ALL_S)))
OBJECTS_C   = $(notdir $(patsubst %.c,%.o,$(ALL_C)))
OBJECTS     = $(addprefix $(TMP_DIR)/, $(OBJECTS_C) $(OBJECTS_CPP)  $(OBJECTS_ASM) $(OBJECTS_S))

#======================== BUILD RULES ====================================
.PHONY:all  clean skip

.DEFAULT_GOAL=default
default: $(TARGET) 

#$(TMP_DIR) $(OUT_DIR)  $(TARGET)

all:  $(TARGET)
	
#	$(MAKE) -j4 DEBUG=y
#	$(MAKE) -j4

	
$(TMP_DIR):
	-mkdir "$(@)"

$(TARGET): $(TMP_DIR) $(OBJECTS) nmc4vars_win.mk Makefile
	$(CC) $(LINKER_FLAGS) $(OBJECTS) $(LIBS)

nmc4vars_win.mk:
	copy "$(NMC_GCC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk" nmc4vars_win.mk

$(OUT_DIR): 
	-mkdir "$(@)"
	
$(TMP_DIR)/%.o: %.asm 
	$(AS) $(AS_FLAGS) $(<) -o$(@) $(INC_DIRS)

$(TMP_DIR)/%.o: %.s 
	$(AS) $(AS_FLAGS) $(<) -o$(@) $(INC_DIRS)
	
$(TMP_DIR)/%.o: %.cpp 
	$(CC) $(CC_FLAGS) -c $(<) -o$(@) $(INC_DIRS) -O0

$(TMP_DIR)/%.o: %.c
	$(CC) $(CC_FLAGS) -c $(<) -o$(@) $(INC_DIRS) -O0
	
	
profiler:
	lua53 $(HAL)/script/profiler-gcc.lua .main0.map > proflist_generated.asm 
	
run: $(TARGET)
	mc12101run $(TARGET)

	

#========================== Visual Studio  ===================
export ALL_CPP ALL_ASM ALL_C ALL_H

PROVIDER = host-x86

$(PROVIDER).vcxproj :	premake5.lua 
	premake5 vs2015

$(PROVIDER).vcproj :	premake5.lua 
	premake5 vs2005

vs2015:   $(PROVIDER).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROVIDER).vcxproj
#	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Release

include $(ROOT)/clean.mk