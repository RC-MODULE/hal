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
	for(int i=0; i<10; i++)
		printf("Hello from NMC 1\n");
	return 0x600D;
}
