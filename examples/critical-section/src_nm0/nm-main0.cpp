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
#include <string.h>
#include "hal_target.h"
#include "sleep.h"
#include "hal.h"
#include "led.h"


extern "C" void halTestCS(int count);

int main()
{
	int* x=(int*)0x000A8000;
	x[0]=0;
	x[1]=0;

	halSetProcessorNo(0);							// Set processor number . Necessary only x86-model
	halHandshake(1);
	halSync(0,1);
	
	halLedOn(0);
	halTestCS(10000000);
	halLedOn(2);
	if (1) for(int i=0; i<10000000;i++){
		halEnterCriticalSectionCore(0);
		x[0]=i;
		if (x[1]){
			printf("%x %x %x",i,x[0],x[1]);
			halLedSOS(0);
		}
		x[0]=0;
		halExitCriticalSectionCore(0);
	}

	halLedOn(4);

	halSync(0,1);
	return 111;
}
