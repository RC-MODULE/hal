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
#   For release nmc build                type 'make'
#   For debug   nmc build                type 'make DEBUG=y'
#   For full  rebuild                    type 'make -B'
#   Multithread build (speed up)         type 'make -j4'
#   To create VisualStudio 2005 project  type 'make vs2005'  (premake5.exe is required)
#   To create VisualStudio 2015 project  type 'make vs2015'  (premake5.exe is required)
#   Run application                      type 'make run'
#   To clean                             type 'make clean'
#   To delete all VS projects &clean     type 'make kill'
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

ROOT             = ../../..
include $(ROOT)/global.mk

# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC5103 board and stores return code in ".exitcode"  file
.SUFFIXES:
PROJECT=vshell

CONFIGURATION ?= Release
OPT ?= -O2
ifdef DEBUG
OPT = -O0 -g 
CONFIGURATION =Debug
endif

VS ?= vs2005
	
#.DEFAULT_GOAL =	vs2015 
#========================== NeuroMatrix build ===================
ifndef MC12101
$(warning *** WARNING: 'MC12101' environment variable is not defined! Board SDK might not have installed correctly. )
endif 

INC_DIRS = -I$(ROOT)/include -I"$(MC12101)/include" -I"$(NEURO)/include" -I"$(HAL)/include"
LIB_DIRS = -L$(ROOT)/lib -L"$(MC12101)/lib" -L$(HAL)/lib -L"$(NEURO)/lib"
LIBS     = mc12101lib_nm.lib libc4.lib hal-mc12101.lib
TARGET   = main.abs
CFG      = mc12101brd-nmc0.cfg
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,../src-nm))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,../src-nm))
HEAPS    =  -heap=122000 -heap1=22000 -heap2=16384 -heap3=16384


$(TARGET): $(SRC_CPP) $(SRC_ASM) $(CFG) Makefile 
	$(info *******************************************************************************)
	$(info **                                                                           **)
	$(info **                       Neuro Matrix <$(CONFIGURATION)> compiling... $(TARGET))
	$(info **                                                                           **)
	$(info *******************************************************************************)
	nmcc -o$(TARGET) -m.main.map $(SRC_CPP) $(SRC_ASM) -nmc4 $(OPT) $(INC_DIRS) $(LIB_DIRS) $(LIBS) -c$(CFG) $(HEAPS)
	@echo ------------------------------------------------------------------------------
	@echo !                            BUILD OK!                                       !
	@echo ------------------------------------------------------------------------------


#========================== Visual Studio build ===================

.PHONY : vs2005 vs2015 run $(VS)
	
vs2005: 
	premake5 vs2005
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).sln  "$(CONFIGURATION)|Win32"
	

vs2015:	
	premake5 vs2015
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).sln /t:Build /p:Configuration=$(CONFIGURATION)

	
$(PROJECT).sln:	premake5.lua 
	premake5 $(MAKECMDGOALS) 

#==================================================================
PATH:=$(MC12101)/bin;$(VSHELL)/bin;$(PATH)

run:
	$(call OSX,./bin/$(CONFIGURATION)/host) 

include $(ROOT)/clean.mk