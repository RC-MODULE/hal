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
#include "sleep.h"
#include "hal.h"
#include "led.h"
#include "time.h"

//DISABLE_PRINTF();
int main()
{
	
	printf("-----------------\n");
	halLedOn(0);
	halSleep(100);
	
	for(int i=0; i<10; i++){
		printf("Hello %d from NMC\n",i);	
		halSleep(100);
	}
	clock();
	halLedOff(0);
	halSleep(500);
	//halSleep(10000);
	return 0x600D;
}
