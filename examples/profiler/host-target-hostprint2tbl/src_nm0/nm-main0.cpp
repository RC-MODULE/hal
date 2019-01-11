//------------------------------------------------------------------------
//
//
//  Copyright (c) RC Module Inc.
//
//!
//! \file   main.cpp
//! \author S. Mushkaev
//! \brief  Example of universl-board-connect usage
//!
//! \endif
//!
//------------------------------------------------------------------------

#include <stdio.h>
#include "hal_target.h"
#include "nmprofiler.h"
#include "sleep.h"
extern "C"{
	volatile int func0(){
		int g=1;
		for (int i=0; i<1000; i++){
			g*=2;
			
		}

		return g;
	}
	volatile int func1(){
		int g=1;
		for (int i=0; i<1000; i++){
			g+=1;
		}
		return g;
	}
	int test();
	
};


int main()
{

	halProfilerEnable();
	for(int i=0; i<20; i++){
	printf("Hello %d from NMC[0]\n",i);	
		func0();
		func1();
		func1();
		halSleep(100);
	}
	
	//halProfilerPrint2tbl(".main.map",0);
	
	return 0x600D;
}





