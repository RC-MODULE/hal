ROOT = ../../..
include $(ROOT)/global.mk
#-include ../local.mk
HEAP ?= -heap=6384 -heap1=16384 -heap2=16384 -heap3=16384
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC5103 board and stores return code in ".exitcode"  file

.SUFFIXES:


TARGET0   = board-target0
TARGET1   = board-target1
HOST      = board-host

ALL: $(TARGET0) $(TARGET1) $(HOST)

$(TARGET0): $(TARGET0).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(TARGET0).vcproj

$(TARGET1): $(TARGET1).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(TARGET1).vcproj
	
$(HOST): $(HOST).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(HOST).vcproj


run: 
	$(MAKE) -j4 runhost runtarget0  runtarget1 

runhost: $(HOST)
	bin\debug\$(HOST)
	
runtarget0: $(TARGET0) 
	bin\debug\$(TARGET0)
	
runtarget1: $(TARGET1) 
	bin\debug\$(TARGET1)
	

clean:
	-$(OS_RM) *.abs *.elf *.ncb *.map *.elf *.suo *.user *.filters
	-$(OS_RD) bin obj obj0 obj1
	
kill: clean
	-$(OS_RM) *.vcproj *.sln *.vcxproj
	
vs2005:	$(HOST).vcproj


$(HOST).vcproj $(TARGET0).vcproj $(TARGET1).vcproj   vs2015:	
	premake5 vs2005

