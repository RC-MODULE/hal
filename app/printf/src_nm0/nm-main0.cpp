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
#include "ubc_target.h"
#include "sleep.h"
int main()
{
	ubcConnect(0);
	for(int i=0; i<100; i++){
		printf("Hello %d from NMC[0]\n",i);	
		halSleep(1000);
	}
	
	
	return 0x600D;
}
