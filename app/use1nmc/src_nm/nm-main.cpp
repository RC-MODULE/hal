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


#include "ubc_target.h"
	
int main()
{
	int* sharedBuffer=ubcMalloc32(2048);// Allocates shared memory (in 32-bit words) 
	//ubcConnect(sharedBuffer,2048);		// Connect and share buffer to host
	if (sharedBuffer==0) return -1;
	unsigned* src = (unsigned*)sharedBuffer;
	unsigned* dst = (unsigned*)sharedBuffer + 1024;
	unsigned  sync,size,incr;
	sync=ubcHostSync(0x6406);			// Handshake 
	size=ubcHostSync((unsigned)src);	// Gets array size, sends input buffer address
	incr=ubcHostSync((unsigned)dst);	// Gets increment (123), sends output buffer address
	// Host >>>>>> Shared memory 		// Here host writes data to shared memory
	sync=ubcHostSync(0x4321);			// Gets ready status (0x600DB00F)
	for (int i = 0; i<size; i++)		// Calcualates output array 
		dst[i] = src[i] + incr;
	ubcHostSync(0x600DBEEF);			// Sends signal that array is modified and ready for reading
	// Host <<<<<< Shared memory 		// Here host reads data from shared memory
	

	int* sharedBuffer2=ubcMalloc32(2048);// Allocates second shared memory (in 32-bit words) 
	for (int i = 0; i<2048; i++)		// Calcualates output array 
		sharedBuffer2[i] = 1234;
	ubcHostSync((unsigned)sharedBuffer2);			// Sends signal that array is modified and ready for reading
	// Host <<<<<< Shared memory 		// Here host reads data from shared memory
	

	ubcDisconnect(sharedBuffer);		// Final sync, dealocates sharedBuffer and disconnects from host
	return 0x600D+(ubcGetProcessorNo()<<28);
}
