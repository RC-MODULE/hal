//------------------------------------------------------------------------
//
//  $Workfile:: main.cpp             $
//
//  <Название библиотеки>
//
//  Copyright (c) RC Module Inc.
//
//  $Revision: 1.1 $      $Date: 2005/03/04 14:35:42 $
//
//! \if file_doc
//!
//! \file   main.cpp
//! \author S. Mushkaev
//! \brief  Example of vec library using
//!
//! \endif
//!
//------------------------------------------------------------------------
#include "hal_target.h"
#include "stdio.h"
#include "led.h"
#include "sleep.h"
#include "hal.h"
	
DISABLE_PRINTF();


 int main()
{

	halLedOn(1);
	halSetProcessorNo(1);
	
	int sharedSize32 =halHostSync(0x10006407);			// Handshake with host/Get shared buffer size
	    sharedSize32 =halHostSync(0x10006408);			// Handshake with host/Get shared buffer size
	int sync=halSync(101,0);							// sync with proc 0

	//---------- memory allocation ---------------------
	int* sharedBuffer1=halMalloc32(sharedSize32+128);	// Allocates shared2 memory (in 32-bit words)  Send array size,
	if (sharedBuffer1==0 ) 
		return -1;

	for(int i=0;i<sharedSize32;i++)		 				// init 
		sharedBuffer1[i]=0;
	
	//---------- exchange shared memory pointers between proc0 and proc 1 -------------
	int* sharedBuffer0;
	sharedBuffer0=(int*)halSyncAddr(sharedBuffer1+8,0);
	
	// nmpu0 >>>> write >>> nmpu1
	halSync(0, 0);		// wait until copied
	for (int i = 0; i<sharedSize32; i++)
		printf("sharedBuffer1[i]=%X\n", sharedBuffer1[i+8]);

	
	//---------- data exchange with host -----------------
	halHostSyncAddr(sharedBuffer1+8);	// Sends sharedBuffer1A address to host
	

	// Host <<< read <<< sharedBuffer1+8			// Here host reads data from shared memory 
	sync=halHostSync(0x1234);						// with ready status (0x600DB00F)
	return 0x600D;
}
