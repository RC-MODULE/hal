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
//#pragma data_section ".data_shared.bss"
//	long srcBuff[1024/2];
//	long dstBuff[1024/2];
	


 int main()
{
	

	int procNo=ubcGetProcessorNo();					// Get processor number 
	int  sharedSize32=2048;							// Set shared buffer size
	int* sharedBuffer=0;							
	int ok=ubcConnect(0,0,&sharedBuffer,&sharedSize32); // Get size and adress of shared memory (in 32-bit words) 
	if (sharedBuffer==0) 
		return -1;
	unsigned* src=(unsigned*)sharedBuffer;
	unsigned* dst=(unsigned*)sharedBuffer+1024;
	unsigned  sync,size,incr;
	sync=ubcHostSync(0x00006407+(procNo<<28));		// Handshake
	size=ubcHostSync((unsigned)src);				// Gets array size, sends input buffer address
	incr=ubcHostSync((unsigned)dst);				// Gets increment (123), sends output buffer address
	// Host >>>>>> Shared memory 					// Here host writes data to shared memory
	sync=ubcHostSync(0x4321);						// Gets ready status (0x600DB00F)
	for(int i=0;i<size;i++) 						// Calcualates output array 
		dst[i]=src[i]+incr;
	ubcHostSync(0x600DBEEF);						// Sends signal that array is modified and ready for reading
	// Host <<<<<< Shared memory 					// Here host reads data from shared memory
	ubcDisconnect(sharedBuffer);					// Final sync, dealocates sharedBuffer and disconnects from host
	return 0x600D+(procNo<<28);
}
