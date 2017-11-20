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
	
	ubcSetProcessorNo(1);
	
	unsigned  sync,incr;

	sync=ubcSync(101,1);							// sync with proc 0

	int  sharedSize32=2048;							// Set shared buffer size
	sync=ubcHostSync(0x10006407);					// Handshake
	int* sharedBuffer0=(int*)ubcHostSync(0);			// Read shared0 memory address 
	int* sharedBuffer1=(int*)ubcHostSync(1);			// Read shared1 memory address
	int* sharedBuffer2=(int*)ubcMalloc32(sharedSize32);	// Allocates shared2 memory (in 32-bit words)  Send array size,
	int* sharedBuffer3=(int*)ubcMalloc32(sharedSize32);	// Allocates shared3 memory (in 32-bit words)  Send array size,
	if (sharedBuffer2==0 || sharedBuffer3==0) 
		return -1;
	
	for(int i=0;i<sharedSize32;i++){ 						
		sharedBuffer2[i]=sharedBuffer1[i];			// direct copy from  processor-0 to processor-1 bank
		sharedBuffer3[i]=0;
	}

	sync=ubcHostSync((unsigned)sharedBuffer2+100);	// Sends sharedBuffer2 address to host
	sync=ubcHostSync((unsigned)sharedBuffer3);		// Sends sharedBuffer3 address to host
	
	
	sync=ubcHostSync(0x4321);						// Gets ready status (0x600DB00F)
	// Host <<<<<< Shared memory 2					// Here host reads data from shared memory 2
	sync=ubcHostSync(0x4321);						// Gets ready status (0x600DB00F)
	// Host >>>>>> Shared memory 3 					// Here host writes data to shared memory
	sync=ubcHostSync(0x4321);						// Gets ready status (0x600DB00F)
	for(int i=0;i<128;i++){ 						// Calcualates output array 
		printf("sharedBuffer1[%d]=%d\n",i,sharedBuffer3[i]);
	}
}
