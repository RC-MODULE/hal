ROOT = ../../..
include $(ROOT)/global.mk
#-include ../local.mk
HEAP ?= -heap=6384 -heap1=16384 -heap2=16384 -heap3=16384
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MB7707 board and stores return code in ".exitcode"  file

.SUFFIXES:


ifndef MB7707
$(error ERROR: 'MB7707' environment variable is not defined!  )
endif 


BOARD    = mb7707
HOST     = $(BOARD)-x86


TARGET   = main.abs
INC_DIRS = -I"$(MB7707)/include" -I$(ROOT)/include -I"$(NEURO)/include"
LIB_DIRS = -L"$(MB7707)/lib" 
LIBS     = mb7707lib.lib libint05.lib
CFG      = mb7707brd.cfg
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,../src_nm))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,../src_nm))


ALL: $(TARGET) $(HOST)
$(TARGET): $(SRC_CPP) $(SRC_ASM) $(CFG) Makefile
	nmcc -o$(TARGET) -m.main.map $(SRC_CPP) $(SRC_ASM)  -nmc3 -g -O0 $(INC_DIRS) $(LIB_DIRS) $(LIBS) -c$(CFG) -heap=16384 -heap1=16384 -heap2=16384 -heap3=16384
$(HOST): $(HOST).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(BOARD)-x86.vcproj



PATH:=$(MB7707)/bin;$(PATH)



run: $(TARGET) $(HOST)
	bin\debug\$(HOST)
	


clean:
	-$(OS_RM) *.abs *.elf *.ncb *.map *.elf *.suo *.user *.filters
	-$(OS_RD) bin obj
	
kill: clean
	-$(OS_RM) *.vcproj *.sln *.vcxproj
	
vs2005:	$(HOST).vcproj

$(HOST).vcproj:
	premake5 vs2005
	