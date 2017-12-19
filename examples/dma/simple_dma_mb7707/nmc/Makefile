
ROOT             =../../../..
ETH_NUMBER       =1
TARGET           =dma.abs
ALL_CPP          =buffers.cpp nm_main.cpp 
LIBS             =hal_7707.lib libint_soc.lib mb7707lib.lib libc05.lib cppnew05.lib
INC_DIRS         =-I"$(NEURO)/include" -I$(ROOT)/libHAL/include -I../common
LIB_DIRS         =-l"$(NEURO)/lib" -l$(ROOT)/libHAL/lib


.SUFFIXES:
OBJECTS=$(notdir $(patsubst %.cpp,%.o,$(ALL_CPP)))


$(TARGET): $(OBJECTS) 
	linker -o$(TARGET) $(OBJECTS) $(LIBS) $(LIB_DIRS) -cmb7707brd.cfg -heap=8000 -heap1=8000 
	
%.o: %.asmx 
	asm -soc -nm2ms  $(<) -o$(@)
	
%.asmx: %.cpp ../common/memcfg.h
	nmcpp -6405 -DNEURO -OPT2 -inline  $(<) -O$(@) $(INC_DIRS) 


run: $(TARGET)
	mb7707run $(TARGET)
#	edcltool -f run_nmc_code.edcl -i $(ETH_NUMBER)
#	$(MAKE) -C ../arm run
	
#	edcltool -f run_nmc_code.edcl -i $(ETH_NUMBER)
#	cd ../arm && make run

dump:
	edcltool -f dump.edcl -i $(ETH_NUMBER)

clean:
	rm -f *.abs *.o