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
	for(int i=0; i<100; i++){
		printf("Hello %d from NMC[1]\n",i);	
		halSleep(100);
	}

	return 0x600D;
}
