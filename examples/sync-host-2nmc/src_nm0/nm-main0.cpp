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
	
int a[1000];

 int main()
{
	int* g = new int[100000];
	int b[1000];
	halLedOn(0);
	
	printf("Hello from nmc[0]\n");
	unsigned  sync;
	halSetProcessorNo(0);							// Set processor number 
	int  sharedSize32=halHostSync(0x00006407);		// Handshake with host /get buffer size
	     sharedSize32=halHostSync(0x00006408);		// Handshake with host /get buffer size
	sync=halSync(100,1);							// Handshake with with proc 1
	

	//---------- memory allocation ---------------------
	//halSetActiveHeap(0);
	int* sharedBuffer0=halMalloc32(sharedSize32+128);	// Allocates shared memory (in 32-bit words) 
	if (sharedBuffer0==0) 
		return -1;
	for(int i=0;i<sharedSize32;i++){ 				// init 
		sharedBuffer0[i]=0xA0000+i;
	}
	
	halHostSyncAddr(sharedBuffer0+16);	// Sends input buffer address
														// Host >>> write >>> sharedBuffer0+16  				// Here host write data to shared memory 0
	sync = halHostSync(0x4321);							// wait until copied 
	for (int i = 0; i<16; i++)
		printf("sharedBuffer0[i]=%X\n", sharedBuffer0[i+16]);
	//---------- exchange shared memory pointers between proc0 and proc 1 -------------
	int* sharedBuffer1 = (int*)halSyncAddr(sharedBuffer0+16, 1);
	
	//---------- copy data from  processor-0 to processor-1
	for(int i=0;i<sharedSize32;i++)
		sharedBuffer1[i]=sharedBuffer0[i+16];			

	halSync(0, 1);		// sync at the end of copy
	//int c = a[10] + b[10]+g[5];
	return 0x600D;
	//return c;
}
