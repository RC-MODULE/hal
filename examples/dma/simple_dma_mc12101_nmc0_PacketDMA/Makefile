CFG = my_mc12101brd1.cfg
NMCC = y
NEURO ?= $(HOME)/nmc4/Bin_old
MC12101 ?= $(HOME)/nmc4/Bin

ifeq ($(OS),Windows_NT)
SHELL  = cmd
else
PATH := $(PATH):$(NEURO)/bin
endif

TARGET      = main.abs
CFG         = my_mc12101brd1.cfg
C_example 	= halInitPacketDMA_exapmple
LIB_DIRS    = -l"$(MC12101)/lib"  -l"$(NEURO)/lib" -l$(ROOT)/lib -l$(HAL)/lib  -l.
LIBS        = mc12101lib_nm.lib libc4f.lib libc4.lib  hal-mc12101.lib cppnew4.lib libint_6407.lib
INC 		    = -I"$(MC12101)/include" -I$(HAL)/include -I"$(NEURO)/include"
ifeq ($(NMCC),y)
$(TARGET):  ../$(C_example).c Makefile $(CFG)
	nmcc ../$(C_example).c -Sc -g -nmc4 -Tc99 $(INC)
	linker $(C_example).elf -o$(TARGET) -c$(CFG) $(LIBS) $(LIB_DIRS) -mmain.map
else 

endif
clean: 
	rm $(TARGET) *.elf *.log *.map

# don't use 'run: $(TARGET)' for remote 
run: $(TARGET)
	mc12101run $(TARGET) 

run-proton: $(TARGET)
	$(RUNDIR)/run
	
perf-proton: perf
	$(RUNDIR)/run

reset-proton:
	$(RUNDIR)/reset12101

