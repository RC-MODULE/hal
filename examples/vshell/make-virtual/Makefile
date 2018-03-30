#============== PROJECT & BUILD CONFIGURATION ==========
#

ROOT             = ../../..
include $(ROOT)/global.mk


CONFIGURATION ?=Release
ifdef DEBUG
CONFIGURATION =Debug
endif

VS ?= vs2005
	
HOST   = host
TARGET = target
#========================== Visual Studio build ===================



.PHONY : vs2005 vs2015 run $(VS)
	
vs2005: $(HOST).sln $(TARGET).sln
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(HOST).sln    "$(CONFIGURATION)|Win32"
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(TARGET).sln  "$(CONFIGURATION)|Win32"
	

vs2015:	$(HOST).sln $(TARGET).sln
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(HOST).sln   /t:Build /p:Configuration=$(CONFIGURATION)
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(TARGET).sln /t:Build /p:Configuration=$(CONFIGURATION)

	
$(HOST).sln $(TARGET).sln:	premake5.lua 
	premake5 $(MAKECMDGOALS) 


	

#========================== Run program ========================================
PATH:=$(VSHELL)/bin;$(PATH)

run:
	$(MAKE) -j2 run-host run-target
	
run-host: 
	$(call OSX,./bin/$(CONFIGURATION)/$(HOST)) 
	
run-target: 
	$(call OSX,./bin/$(CONFIGURATION)/$(TARGET)) 

include $(ROOT)/clean.mk