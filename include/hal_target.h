///////////////////////////№вуашту ///////////////////////////////////////////////
//                                                                      //
// mc5103_load.h -                                                      //
//                                                                      //
//      Header file for synchronization and exchange library            //
//                                                                      //
// Copyright (c) 2003 RC Module                                         //
//      If this code works, it was written by Alex Ruzavin              //
//      If not, I don't know who wrote it.                              //
//                                                                      //
// $Revision:: 1     $      $Date:: 2/16/06 4:07p    $                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef _HAL_TARGET_H
#define _HAL_TARGET_H

#include <stdlib.h>



extern "C"
{



	int halSyncArray(
					 int value,        // Sync value
					 void *outAddress, // Sended array address (can be NULL)
					 size_t outLen,    // Sended array length (can be 0)
					 void **inAddress, // Received array address pointer (can be NULL)
					 size_t *inLen,   // Received array size pointer (can be NULL)
					 int  procNo);

	int halSync(int val,int processor=0);
	int halSyncAddr(int sendAddr,int processor);
	// Return processor number on board
	int ncl_getProcessorNo(void);

	// Barrier sync with host PC
	int ncl_hostSync(int value);

	// Barrier sync with host PC with array exchange
	int ncl_hostSyncArray(
		int value,        // Sync value
		void *outAddress, // Sended array address (can be NULL)
		size_t outLen,    // Sended array length (can be 0)
		void **inAddress, // Received array address pointer (can be NULL)
		size_t *inLen);   // Received array size pointer (can be NULL)
	void halSetProcessorNo(int number);

	#ifndef __NM__
	int* halMalloc32(int sharedSize32);
	void halFree(int* );
	#else 
		//extern void*    halSharedBuffer;
		//extern unsigned halSharedSize;
		//#include <string.h>
		inline int* halMalloc32(int sharedSize32) {return (int*)malloc(sharedSize32);}
		inline void halFree(int* shared) { free(shared);}
	#endif

};

//int halSetProcessorNo(int);
#ifndef PROCESSOR_ID
	#ifdef __NM__
	#define halGetProcessorNo() ncl_getProcessorNo()

	#else 
	#define PROCESSOR_ID 0
	#define halGetProcessorNo() PROCESSOR_ID
	#endif
#else 
	#define halGetProcessorNo() PROCESSOR_ID
#endif

// Barrier sync with host PC
#define halHostSync(value) ncl_hostSync(value)


#ifdef __NM__

int halHostSyncArray(
		int value,        // Sync value
		void *outAddress, // Sended array address (can be NULL)
		size_t outLen,    // Sended array length (can be 0)
		void **inAddress, // Received array address pointer (can be NULL)
		size_t *inLen);   // Received array size pointer (can be NULL)


int halHostSyncMemory(
		int value,        // Sync value
		void *outAddress, // Sended array address (can be NULL)
		size_t outLen,    // Sended array length (can be 0)
		void **inAddress, // Received array address pointer (can be NULL)
		size_t *inLen);   // Received array size pointer (can be NULL)

		
//int halDisconnect(int* sharedBuffer=0) {if (sharedBuffer) free(sharedBuffer); return 1;} 
int halInterConnect(int* shared);
#else 
//int halConnect(int* masterSharedBuffer=0, int masterSharedSize32=0, int** sharedBuffer=0, int* sharedSize32=0);
//int halDisconnect(int* sharedBuffer=0);
#endif 




#endif  // _UBC_TARGET_H