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
#include "ubc_target.h"
#include "stdio.h"
#include "led.h"
#include "sleep.h"
//#pragma data_section ".data_shared.bss"
//	long srcBuff[1024/2];
//	long dstBuff[1024/2];
	





 int main()
{

	halLedOn(1);
	ubcSetProcessorNo(1);
	
	unsigned  sync,incr;

	

//---------- memory allocation ---------------------
	sync=ubcHostSync(0x10006407);					// Handshake
	sync=ubcSync(101,0);							// sync with proc 0
	sync=ubcHostSync(sync);							// Send reply from proc 0

	//---------- memory allocation ---------------------
	int  sharedSize32=2048;							// Set shared buffer size
	int* sharedBuffer1A=ubcMalloc32(sharedSize32);	// Allocates shared2 memory (in 32-bit words)  Send array size,
	int* sharedBuffer1B=ubcMalloc32(sharedSize32);	// Allocates shared3 memory (in 32-bit words)  Send array size,
	if (sharedBuffer1A==0 || sharedBuffer1B==0) 
		return -1;

	for(int i=0;i<sharedSize32;i++){ 				// init 
		sharedBuffer1A[i]=0xB0000+i;
		sharedBuffer1B[i]=0;
	}
	//---------- exchange shared memory pointers between proc0 and proc 1 -------------
	int* sharedBuffer0A;
	int* sharedBuffer0B;
	int  bufferSize0A;
	int  bufferSize0B;
	sync = ubcSyncArray(0x1A,sharedBuffer1A,sharedSize32,(void**)&sharedBuffer0A,(size_t*)&bufferSize0A,0);			// Read shared0 memory address 
	sync = ubcSyncArray(0x1B,sharedBuffer1B,sharedSize32,(void**)&sharedBuffer0B,(size_t*)&bufferSize0B,0);			// Read shared0 memory address 
	
	//---------- direct copy from  processor-1 to processor-0
	for(int i=0;i<sharedSize32;i++)						
		sharedBuffer1B[i]=sharedBuffer0A[i];			// direct copy from  processor-0 to processor-1 bank
	
	for(int i=0;i<16;i++)
		printf("sharedBuffer1B[i]=%X\n",sharedBuffer1B[i]);

	//---------- data exchange with host -----------------
	sync=ubcHostSync(33);							// Gets ready status (0x600DB00F)
	sync=ubcHostSync((unsigned)(sharedBuffer1A+16));	// Sends sharedBuffer1A address to host
	sync=ubcHostSync((unsigned)(sharedBuffer1B+4));	// Sends sharedBuffer1B address to host
	
	

	// Host <<<<<< sharedBuffer1A+16				// Here host reads data from shared memory 2
	sync=ubcHostSync(0x4321);						// Gets ready status (0x600DB00F)
	// Host >>>>>> sharedBuffer1B+4					// Here host writes data to shared memory
	sync=ubcHostSync(0x4321);						// Gets ready status (0x600DB00F)
	for(int i=0;i<16;i++){ 						// Calcualates output array 
		printf("sharedBuffer1B[%d]=%X\n",i,sharedBuffer1B[i]);
	}
	while(1);
	return 0x600D;
}
