ROOT = ../../..
include $(ROOT)/global.mk
#-include ../local.mk
HEAP ?= -heap=6384 -heap1=16384 -heap2=16384 -heap3=16384
# BUILD AND RUN MAKEFILE
# This makefile builds simple nmc-application containing single cpp-file  
# Rule 'run' executes the application on MC5103 board and stores return code in ".exitcode"  file

.SUFFIXES:


ifndef MC7601
$(error ERROR: 'MC7601' environment variable is not defined!  )
endif 

ifndef MC7601IP
$(error ERROR: 'MC7601IP' environment variable is not defined!  )
endif 

BOARD    = mc7601
HOST     = $(BOARD)-x86


TARGET   = main.abs
INC_DIRS = -I"$(MC7601)/include" -I$(ROOT)/include -I"$(NEURO)/include"
LIB_DIRS = -L"$(MC7601)/lib" -L$(ROOT)/lib 
LIBS     = mc7601_ll_nm.lib libint_soc.lib hal-mc7601.lib
CFG      = mc7601brd.cfg
SRC_CPP  = $(wildcard $(addsuffix /*.cpp,../src_nm0))
SRC_ASM  = $(wildcard $(addsuffix /*.asm,../src_nm0))


ALL: $(TARGET) $(HOST)

$(TARGET): $(SRC_CPP) $(SRC_ASM) $(CFG) Makefile
	nmcc -o$(TARGET) -m.main.map $(SRC_CPP) $(SRC_ASM)  -soc -g -O0 $(INC_DIRS) $(LIB_DIRS) $(LIBS) -c$(CFG) -heap=16384 -heap1=16384 -heap2=16384 -heap3=16384

$(HOST): $(HOST).vcproj
	"$(VS80COMNTOOLS)vsvars32" && vcbuild $(BOARD)-x86.vcproj



PATH:=$(MC7601)/bin;$(PATH)



run: $(TARGET) $(HOST)
		mc7601run ./$(TARGET) 
	


clean:
	-$(OS_RM) *.abs *.elf *.ncb *.map *.elf *.suo *.user *.filters
	-$(OS_RD) bin obj
	
kill: clean
	-$(OS_RM) *.vcproj *.sln *.vcxproj
	
vs2005:	$(HOST).vcproj

$(HOST).vcproj:
	premake5 vs2005
	