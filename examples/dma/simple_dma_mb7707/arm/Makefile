DEFTUBE = -DCTUBE_ON
ifdef CTUBE_ON
	DEFTUBE = -DCTUBE_ON
endif  
ifdef CTUBE_OFF
	DEFTUBE = -DCTUBE_OFF
endif  

GAS =$(CODESOURCERY)/bin/arm-none-eabi-as
GCC =$(CODESOURCERY)/bin/arm-none-eabi-gcc
GLD =$(CODESOURCERY)/bin/arm-none-eabi-gcc
GAR =$(CODESOURCERY)/bin/arm-none-eabi-ar
OBJCOPY = $(CODESOURCERY)/bin/arm-none-eabi-objcopy
OBJDUMP = $(CODESOURCERY)/bin/arm-none-eabi-objdump
#CCFLAGS = -Wall -O0 -mcpu=arm1176jzf-s -DGPIO_LEG=6 -fno-zero-initialized-in-bss -I$(SRC) -I./ -I/opt/pcad/CodeSourcery/Sourcery_G++_Lite/arm-none-eabi/include $(DEFAUTO) $(DEFPACKAGE) $(DEFTUBE)
#-fno-rtti
# -Map map.txt --dynamic-list-data --dynamic-list-cpp-new   --dynamic-list-cpp-typeinfo  

SRC	= ./
CCFLAGS =  -fno-exceptions -Wall -O0 -mcpu=arm1176jzf-s -DGPIO_LEG=6 -fno-zero-initialized-in-bss -I$(SRC) -I. -I../common -I../../include  -I$(CODESOURCERY)/Sourcery_G++_Lite/arm-none-eabi/include $(DEFAUTO) $(DEFPACKAGE) $(DEFTUBE)
ASMFLAGS= -I$(SRC) -I./  $(DEFASMTUBE)
LFLAGS  = 
LIB_DIRS = -L./ -L$(CODESOURCERY)/lib/gcc/arm-none-eabi/4.5.1 -L$(CODESOURCERY)/arm-none-eabi/lib  

PROJECT= test_image
TARGET = $(PROJECT).bin
ASRCS  =  \
	$(SRC)/init_VIC_asm.S  \
	$(SRC)/init_Cache_asm.S 

CSRCS  = ddr.c 

EXEC_CSRCS = main.cpp 

EXEC_ASRCS = image_start.S 

COBJS_= $(subst $(SRC)/,,$(CSRCS))
COBJS = $(subst .c,.o,$(COBJS_))

AOBJS_= $(subst $(SRC)/,,$(ASRCS))
AOBJS = $(subst .S,.o,$(AOBJS_))

EXEC_COBJS = $(subst .cpp,.o,$(EXEC_CSRCS)) 

EXEC_AOBJS = $(subst .S,.o,$(EXEC_ASRCS)) 

	
all: $(TARGET)

$(AOBJS) : $(ASRCS)
	@echo "************* ASM COMPILATION **********************"
	$(GCC) $(ASMFLAGS) -c $(ASRCS) 
	
$(COBJS) : $(CSRCS)
	@echo "************* C COMPILATION **********************"
	$(GCC) $(CCFLAGS) -c $(CSRCS) 

$(EXEC_COBJS) : $(EXEC_CSRCS) 
	@echo "************* EXEC C COMPILATION **********************"
	$(GCC) $(CCFLAGS) -c $(EXEC_CSRCS) 

$(EXEC_AOBJS) : $(EXEC_ASRCS)
	@echo "************* EXEC ASM COMPILATION **********************"
	$(GCC) $(ASMFLAGS) -c $(EXEC_ASRCS) 

libtests.a: $(COBJS) $(AOBJS)	
	$(GAR) rcs libtests.a $(COBJS) $(AOBJS)
	
$(TARGET)	: libtests.a $(EXEC_COBJS) $(EXEC_AOBJS) 
	@echo "************* LINKAGE **********************"
	$(GLD) $(LFLAGS) $(EXEC_AOBJS) $(EXEC_COBJS) -ltests -lc  -lcs3hosted  $(LIB_DIRS)  -T main.lds  -o $(PROJECT).elf
	$(OBJCOPY) -O binary $(PROJECT).elf $(TARGET)
	$(OBJDUMP) -D $(PROJECT).elf > $(PROJECT).dmp
	
ETH_NUMBER=0

.PHONY= clean load start nmrun run dump
clean:
	rm --force $(EXEC_AOBJS) $(EXEC_COBJS) $(COBJS) $(AOBJS) test_image.bin test_image.elf libtests.a linkmap.txt
	rm --force test_image.dmp
	@echo "Tests are cleaned"

load: $(TARGET) load.edcl
	edcltool -f ./load.edcl -i $(ETH_NUMBER)

start: $(TARGET)	
	edcltool -f ./start.edcl -i $(ETH_NUMBER)

nmrun: $(TARGET)
	$(MAKE) -C ../nmc run
	
run: $(TARGET)
	edcltool -f ./load.edcl -i $(ETH_NUMBER)
	edcltool -f ./start.edcl -i $(ETH_NUMBER)
	-$(MAKE) -C ../nmc run
	edcltool -f ./dump.edcl -i $(ETH_NUMBER)

dump: 
	edcltool -f ./dump.edcl -i $(ETH_NUMBER);	

	

	
	
#-sudo edcltool -f load.edcl -i 1	 
#-cd ../nm; sudo edcltool -f ./load.edcl -i eth1;	sudo edcltool -f ./run.edcl -i eth1; cd ../arm	
