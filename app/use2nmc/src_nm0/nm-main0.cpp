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
//#pragma data_section ".data_shared.bss"
//	long srcBuff[1024/2];
//	long dstBuff[1024/2];
	


 int main()
{
	
	unsigned  sync;
	ubcSetProcessorNo(0);							// Set processor number 
	sync= ubcSync(100,1);							// sync with proc 1
	int  sharedSize32=2048;							// Set shared buffer size
	int* sharedBuffer0=ubcMalloc32(sharedSize32);	// Allocates shared memory (in 32-bit words) 
	int* sharedBuffer1=ubcMalloc32(sharedSize32);	// Allocates shared memory (in 32-bit words) 
	if (sharedBuffer0==0 || sharedBuffer1==0) 
		return -1;
	for(int i=0;i<sharedSize32;i++){ 				// init 
		sharedBuffer0[i]=i;
		sharedBuffer1[i]=0;
	}

	
	sync=ubcHostSync(0x00006407);					// Handshake
	sync=ubcHostSync((unsigned)sharedSize32);		// Send array size,
	sync=ubcHostSync((unsigned)sharedBuffer0);		// Sends input buffer address
	sync=ubcHostSync((unsigned)sharedBuffer1);		// sends output buffer address
	// Host <<<<<< Shared0 memory 					// Here host reads data from shared memory
	sync=ubcHostSync(0x4321);						// Gets ready status (0x600DB00F)
	// Host >>>>>> Shared1 memory 					// Here host writes data to shared memory
	sync=ubcHostSync(0x4321);						// Gets ready status (0x600DB00F)
	for(int i=0;i<128;i++){ 						// Calcualates output array 
		printf("sharedBuffer1[%d]=%d\n",i,sharedBuffer1[i]);
	}
	
	ubcDisconnect(0);					// Final sync, dealocates sharedBuffer and disconnects from host
	return 0x600D;
}
