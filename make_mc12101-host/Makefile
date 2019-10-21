ROOT = ..
-include $(ROOT)/global.mk
.SUFFIXES:

PROJECT  = hal-mc12101

	
#$(MAKECMDGOALS) 
vs2015:
	premake5 vs2015
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Release /property:Platform=win32
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Debug   /property:Platform=win32
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Release /property:Platform=x64
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Debug   /property:Platform=x64
#"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).sln /t:Build /p:Configuration=$(CONFIGURATION)


vs2005: $(PROJECT).sln
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).sln  "$(CONFIGURATION)|Win32"

	
$(PROJECT).sln:	premake5.lua 
	premake5 $(MAKECMDGOALS) 
	
#--------------- cleanup  --------------------------------	


-include $(ROOT)/clean.mk