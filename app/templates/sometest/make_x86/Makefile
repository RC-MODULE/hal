

include $(ROOT)/global.mk
	   
PROJECT = test-x86
TARGET  = $(call OSX,bin/Release/$(PROJECT))

vs2015:    	$(PROJECT).vcxproj
#	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj
	"$(VS140COMNTOOLS)vsvars32" && msbuild $(PROJECT).vcxproj /p:Configuration=Release

	
vs2005:    	$(PROJECT).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(PROJECT).vcproj

$(TARGET): vs2015
	
run: vs2015	
	$(TARGET)
	
test: .exitcode
	
.exitcode: $(TARGET) .test.bat
	.test.bat 
	
.test.bat: 
	@echo $(TARGET) >.test.bat
	@echo @echo %%errorlevel%% ^>.exitcode >>.test.bat


$(PROJECT).vcxproj :	premake5.lua 
	premake5 vs2015

	
$(PROJECT).vcproj :	premake5.lua 
	premake5 vs2005

include $(ROOT)/clean.mk	