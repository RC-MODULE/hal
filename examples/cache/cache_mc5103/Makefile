ROOT     = ../../..
PROJECT  =simple_dma_mc5103
TARGET   =$(PROJECT).abs
INC_DIRS = -I"$(NEURO)/include" -I$(ROOT)/include 
LIB_DIRS = -L"$(NEURO)/lib" -L$(ROOT)/lib -L$(MC5103)/lib
LIBS     = hal-mc5103.lib libint05.lib mc5103lib.lib libc05.lib cppnew05.lib
CFG      = mc5103brd.cfg
ALL_CPP  = ../nm_main.cpp 
HEAP     = -heap=0 -heap1=0 -heap2=0 -heap3=0
.SUFFIXES:
OBJECTS=$(notdir $(patsubst %.cpp,%.o,$(ALL_CPP)))




$(TARGET): $(ALL_CPP)
	nmcc -nmc3 -o$(TARGET) -m.main.map $(ALL_CPP)  -g -O0 $(INC_DIRS) $(LIB_DIRS) $(LIBS) -c$(CFG) $(HEAP)
	
#linker -o$(TARGET) $(OBJECTS) $(LIBS) $(LIB_DIRS) -cmc5103brd.cfg -heap=8000 -heap1=8000 

run: $(TARGET)
	mc5103run $(TARGET)

emurun: $(TARGET)
	nm6405emurun $(TARGET)
		
clean:
	rm -f *.abs *.elf *.o *.map
	
	
	