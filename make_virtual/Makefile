ROOT = ..
include $(ROOT)/global.mk
#-include ../local.mk

.SUFFIXES:
PROJECT  = hal-virtual
PROJECT_X86   = $(PROJECT)-x86
TARGET_X86 = $(OUT_DIR)/$(PROJECT_X86).lib
OUT_DIR = ../lib


#--------------- X86 build --------------------------------	
x86: $(VS_VER)

	
vs2005: $(PROJECT_X86).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT_X86).vcproj "Debug"

vs2015:	$(PROJECT_X86).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT_X86).vcxproj /p:Configuration=Debug
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT_X86).vcxproj /p:Configuration=Release
	
$(PROJECT_X86).vcproj:	premake5.lua 
	premake5 vs2005

$(PROJECT_X86).vcxproj :	premake5.lua 
	premake5 vs2015
	
#--------------- cleanup  --------------------------------	
-include $(ROOT)/clean.mk