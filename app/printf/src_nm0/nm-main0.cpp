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

//DISABLE_PRINTF();
int main()
{
	for(int i=0; i<10; i++){
		printf("Hello %d from NMC[0]\n",i);	
		halSleep(500);
	}
	
	halSleep(10000);
	return 0x600D;
}
