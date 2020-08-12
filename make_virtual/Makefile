ROOT = ..
include $(ROOT)/global.mk
#-include ../local.mk

.SUFFIXES:
PROJECT  = hal-virtual
PROJECT_X86   = $(PROJECT)
TARGET_X86 = $(OUT_DIR)/$(PROJECT_X86).lib
OUT_DIR = ../lib


#--------------- X86 build --------------------------------	
x86: $(VS_VER)

	
vs2005: $(PROJECT).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).vcproj "Debug"

vs2015:	$(PROJECT).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Release /property:Platform=win32
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Debug   /property:Platform=win32
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Release /property:Platform=x64
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Debug   /property:Platform=x64
	
	
	
$(PROJECT_X86).vcproj:	premake5.lua 
	premake5 vs2005

$(PROJECT_X86).vcxproj :	premake5.lua 
	premake5 vs2015
	
#--------------- cleanup  --------------------------------	
-include $(ROOT)/clean.mk