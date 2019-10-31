#
# This is a  Makefile for NeuroMatrix projects (application or library) 
# with automatic recursive search of source files , compiling and linking.
# Makefile is processed by GNU make and may be used both in Linux and Windows OS
#
# USAGE:
#   For release build                    run 'make'
#   For debug   build                    run 'make DEBUG=y'
#   For debug & release  build           run 'make all'
#   For full  rebuild                    run 'make -B'
#   Multithread build (speed up)         run 'make -j4'
#   To clean                             run 'make clean'
#   To clean DEBUG & RELEASE             run 'make cleanall'
#   To create VisualStudio 2005 project  run 'make vs2005'
#   To create VisualStudio 2015 project  run 'make vs2015'
#   For verbose build (no colorizing)    run 'make VERBOSE=y'
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
#   
#   CFLAGS          - C compiler
#   CXXFLAGS        - C++ compiler flags   
#   AS              - assembler
#   ASMFLAGS        - assembler flags for compiling of pure .asm files 
#   AR              - librarian
#   ARFLAGS         - librarian flags of (libr/linker)
#

ROOT             = ..
ifeq ($(OS),Windows_NT)
nmc4vars_win.mk:
	-copy "$(NMC_GCC_TOOLPATH)\nmc4-ide\include\nmc4vars_win.mk" nmc4vars_win.mk

-include 		 nmc4vars_win.mk
SHELL 			 = cmd
endif

DEBUG = y;


CONFIGURATION ?= Release
ifdef DEBUG
CONFIGURATION =Debug
else
$(error Release configuration cause bug in printf. Run with flag DEBUG=y )
endif


#--------------  RELEASE/ALL config -------------------


ARCH            = nmc4
PROJECT  = hal-mc12101
TMP_DIR  = $(CONFIGURATION)
OUT_DIR  = $(ROOT)/lib
TARGET          = $(OUT_DIR)/lib$(PROJECT).a
CC       		= nmc-g++
AS              = asm 				 
AR               = nmc-ar
CXXFLAGS 		= -std=c++11 -O2 -Wall -m$(ARCH) -c

ASFLAGS        = -$(ARCH) -nm2ms  $(INC_DIRS) -split_sir -W-111 -W-109

ARFLAGS          = rcs $(TARGET)

#-------------- DEBUG config -------------------------
ifdef DEBUG
ASFLAGS         += -g
endif 



INC_DIRS = -I$(ROOT)/include -I../src_1879vm6ya

SRC_DIRS = ../src_1879vm6ya ../src_nm_io ../src_ringbuffer ../src_nmc_all ../src_printf_nmgcc
ALL_CPP  = $(wildcard $(addsuffix /*.cpp,$(SRC_DIRS)))
ALL_ASM  = $(wildcard $(addsuffix /*.asm,$(SRC_DIRS)))
ALL_C    = $(wildcard $(addsuffix /*.c  ,$(SRC_DIRS)))
ALL_S    = $(wildcard $(addsuffix /*.s,  $(SRC_DIRS)))





#################################################

VPATH    = $(SRC_DIRS)

# collecting of objects
OBJECTS_CPP = $(notdir $(patsubst %.cpp,%.o,$(ALL_CPP)))
OBJECTS_ASM = $(notdir $(patsubst %.asm,%.o,$(ALL_ASM)))
OBJECTS_C   = $(notdir $(patsubst %.c,%.o,$(ALL_C)))
OBJECTS_S   = $(notdir $(patsubst %.s,%.o,$(ALL_S)))
export OBJECTS     = $(addprefix $(TMP_DIR)/,$(OBJECTS_C) $(OBJECTS_CPP)  $(OBJECTS_ASM) $(OBJECTS_S))





#========================== NeuroMatrix build ===================	
.DEFAULT_GOAL:=$(TARGET)

default: 
	$(MAKE) $(TARGET) 

logo:
	$(info *******************************************************************************)
	$(info **                                                                           **)
	$(info **                       Neuro Matrix <$(CONFIGURATION)> compiling...          )
	$(info **                                                                           **)
	$(info *******************************************************************************)


$(TARGET): logo $(TMP_DIR) $(OUT_DIR) $(OBJECTS) 
	$(AR) $(ARFLAGS) $(OBJECTS)
	@echo *******************************************************************************
	@echo **                                                                           **
	@echo **                                   Build OK!                               **
	@echo **                                                                           **
	@echo *******************************************************************************

legacy:
	$(MAKE) -f legacy.mk
	
#$(TMP_DIR)/printf.o: ../src_printf_nmgcc/printf.c
#	$(CC) ../src_printf_nmcpp/printf.c $(INC_DIRS) $(CXXFLAGS) -o$(TMP_DIR)/printf.o 

$(TMP_DIR)/printf.o: ../src_printf_nmgcc/printf.cpp
	$(CC) -std=c++11 -O0 -Wall -m$(ARCH) -c $(<) -o$(@) $(INC_DIRS) $(ERRECHO) 


$(TMP_DIR):
	-mkdir "$(@)"

$(OUT_DIR): 
	-mkdir "$(@)"
	
$(TMP_DIR)/%.o: %.c
	$(CC) $(CXXFLAGS) $(<) -o$(@) $(INC_DIRS) $(ERRECHO)

	
$(TMP_DIR)/%.o: %.cpp 
	$(CC) $(CXXFLAGS) $(<) -o$(@) $(INC_DIRS) $(ERRECHO)

$(TMP_DIR)/%.o: %.asm 
	$(AS) $(ASFLAGS) $(<) -o$(@) $(ERRECHO)

$(TMP_DIR)/%.o: %.s 
	$(AS) $(ASFLAGS) $(<) -o$(@) $(ERRECHO)

#========================== Visual Studio build ===================	

	
vs2005: $(PROJECT).sln
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).sln  "$(CONFIGURATION)|Win32"
	

vs2015:	$(PROJECT).sln
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).sln /t:Build /p:Configuration=$(CONFIGURATION)

	
$(PROJECT).sln:	premake5.lua 
	premake5 $(MAKECMDGOALS) 
	
#--------------- cleanup  --------------------------------	


-include $(ROOT)/clean.mk