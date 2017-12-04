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
//#pragma data_section ".data_shared.bss"
//	long srcBuff[1024/2];
//	long dstBuff[1024/2];
	


 int main()
{
	
	halLedOn(0);
	
	printf("Hello from nmc[0]\n");
	unsigned  sync;
	halSetProcessorNo(0);							// Set processor number 
	sync=halHostSync(0x00006407);					// Handshake with host
	sync=halSync(100,1);							// Handshake with with proc 1
	sync=halHostSync(sync);							// Repost handshake from proc 1 to Host


	//---------- memory allocation ---------------------
	int  sharedSize32=2048;							// Set shared buffer size
	halSetActiveHeap(0);
	int* sharedBuffer0A=halMalloc32(sharedSize32);	// Allocates shared memory (in 32-bit words) 
	halSetActiveHeap(1);
	int* sharedBuffer0B=halMalloc32(sharedSize32);	// Allocates shared memory (in 32-bit words) 
	if (sharedBuffer0A==0 || sharedBuffer0B==0) 
		return -1;
	for(int i=0;i<sharedSize32;i++){ 				// init 
		sharedBuffer0A[i]=0xA0000+i;
		sharedBuffer0B[i]=0;
	}
	//---------- exchange shared memory pointers between proc0 and proc 1 -------------
	int* sharedBuffer1A;
	int* sharedBuffer1B;
	int  bufferSize1A;
	int  bufferSize1B;
	sync = halSyncArray(0x0A,sharedBuffer0A,sharedSize32,(void**)&sharedBuffer1A,(size_t*)&bufferSize1A,1);			// Read shared0 memory address 
	sync = halSyncArray(0x0B,sharedBuffer0B,sharedSize32,(void**)&sharedBuffer1B,(size_t*)&bufferSize1B,1);			// Read shared0 memory address 
	
	//---------- direct copy from  processor-1 to processor-0
	for(int i=0;i<sharedSize32;i++)
		sharedBuffer0B[i]=sharedBuffer1A[i];			

	for(int i=0;i<16;i++)
		printf("sharedBuffer0B[i]=%X\n",sharedBuffer0B[i]);


	//---------- data exchange with host -----------------
	sync=halHostSync((unsigned)32);		// Send array size,
	sync=halHostSync((unsigned)(sharedBuffer0A+16));	// Sends input buffer address
	sync=halHostSync((unsigned)(sharedBuffer0B+4));		// sends output buffer address
	// Host <<<<<< sharedBuffer0A+16  				// Here host reads data from shared memory
	sync=halHostSync(0x4321);						// Gets ready status (0x600DB00F)
	// Host >>>>>> SharedBuffer0B+4 					// Here host writes data to shared memory
	sync=halHostSync(0x1234);						// Gets ready status (0x600DB00F)
	for(int i=0;i<16;i++)
		printf("sharedBuffer0B[%d]=%X\n",i,sharedBuffer0B[i]);
	
	while(1);
	return 0x600D;
}
