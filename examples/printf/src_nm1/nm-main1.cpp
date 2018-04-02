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
#include "cache.h"
//DISABLE_PRINTF();

int main()
{
	for(int i=0; i<10; i++){
		halInstrCacheEnable();
		printf("Hello %d from NMC[1]\n",i);	
		halSleep(100);
	}

	return 0x600D;
}
