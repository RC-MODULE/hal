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
#include "printx.h"

//DISABLE_PRINTF();
int main()
{
	char str[10];
	//sprintf(str,"\n");
	printf("\r\n");
	printf("-----------------\n");
	halLedOn(0);
	halSleep(100);

	int d=4;	
	for(int i=0; i<10; i++){
		printf("Hello %d from NMC\n",i);	
		halSleep(100);
		d*=d;
		d+=d;
		d>>=d;
	}
	clock();
	
	int mx[4][3];
	for(int i=0; i<4; i++)
		for(int j=0; j<3; j++)
			mx[i][j]=i*16+j;
	
	PRINT_MATRIX_32S("mx",mx,4,3);
		
	halLedOff(0);
	halSleep(500);
	//halSleep(10000);
	return d;//0x600D;
}
