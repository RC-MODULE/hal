ROOT = ../../../..
include $(ROOT)/global.mk
#-include ../local.mk
HEAP ?= -heap=6384 -heap1=16384 -heap2=16384 -heap3=16384
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC5103 board and stores return code in ".exitcode"  file

.SUFFIXES:


TARGET   = board-target
HOST     = board-host

ALL: $(TARGET) $(HOST)

$(TARGET): $(TARGET).vcproj



run: $(TARGET) $(HOST)
	$(MAKE) -j2 runhost runtarget
	
runhost: $(HOST)
	bin\debug\$(HOST)
	
runtarget: $(TARGET)
	bin\debug\$(TARGET)

clean:
	-$(OS_RM) *.abs *.elf *.ncb *.map *.elf *.suo *.user *.filters *.db
	-$(OS_RD) bin obj Debug Release
	
kill: clean
	-$(OS_RM) *.vcproj *.sln *.vcxproj
	
vs2005:	$(HOST).vcproj
	premake5 vs2005
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(TARGET).vcproj

vs2015:	
	premake5 vs2015
