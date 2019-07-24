
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

	halSetProcessorNo(1);							// Set processor number 
	halHandshake(0);
	halSync(777,0);
	halLedOn(1);
	halTestCS(10000000);
	halLedOn(3);
	
	if (1) for(int i=0; i<10000000;i++){
		halEnterCriticalSectionCore(1);
		x[1]=i;
		if (x[0]){
			halLedSOS(1);
		}
		x[1]=0;
		halExitCriticalSectionCore(1);
	}

	halLedOn(5);
	halSync(0,0);

	return 111;
}
