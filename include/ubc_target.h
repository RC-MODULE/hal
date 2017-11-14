///////////////////////////№вуашту ///////////////////////////////////////////////
//                                                                      //
// mc5103_load.h -                                                      //
//                                                                      //
//      Header file for synchronization and exchange library for MC5103 //
//                                                                      //
// Copyright (c) 2003 RC Module                                         //
//      If this code works, it was written by Alex Ruzavin              //
//      If not, I don't know who wrote it.                              //
//                                                                      //
// $Revision:: 1     $      $Date:: 2/16/06 4:07p    $                  //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef _UBC_TARGET_H
#define _UBC_TARGET_H

#include <stdlib.h>



extern "C"
{
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
};

//int ubcSetProcessorNo(int);
#ifndef PROCESSOR_ID
	#ifdef __NM__
	#define ubcGetProcessorNo() ncl_getProcessorNo()

	#else 
	#define PROCESSOR_ID 0
	#define ubcGetProcessorNo() PROCESSOR_ID
	#endif
#else 
	#define ubcGetProcessorNo() PROCESSOR_ID
#endif

// Barrier sync with host PC
#define ubcHostSync(value) ncl_hostSync(value)


#ifdef __NM__
int ubcConnect(int* masterSharedBuffer,int masterSharedSize32=0, int** sharedBuffer=0, int* sharedSize32=0){
	if (sharedSize32==0){
		ubcHostSync((int)masterSharedBuffer);
		ubcHostSync((int)masterSharedSize32);
	}
	else {
		*sharedBuffer=(int*)ubcHostSync(111);
		*sharedSize32=ubcHostSync(222);
	}
	return 1;
}
/*
int ubcHostConnect(){
	if (sharedSize32==0){
		ubcHostSync((int)masterSharedBuffer);
		ubcHostSync((int)masterSharedSize32);
	}
	else {
		*sharedBuffer=(int*)ubcHostSync(111);
		*sharedSize32=ubcHostSync(222);
	}
	return 1;
}*/
int ubcHostSyncArray(
		int value,        // Sync value
		void *outAddress, // Sended array address (can be NULL)
		size_t outLen,    // Sended array length (can be 0)
		void **inAddress, // Received array address pointer (can be NULL)
		size_t *inLen);   // Received array size pointer (can be NULL)


int ubcHostSyncMemory(
		int value,        // Sync value
		void *outAddress, // Sended array address (can be NULL)
		size_t outLen,    // Sended array length (can be 0)
		void **inAddress, // Received array address pointer (can be NULL)
		size_t *inLen);   // Received array size pointer (can be NULL)

		
int ubcDisconnect(int* sharedBuffer=0) {if (sharedBuffer) free(sharedBuffer); return 1;} 
int ubcInterConnect(int* shared);
#else 
int ubcConnect(int* masterSharedBuffer=0, int masterSharedSize32=0, int** sharedBuffer=0, int* sharedSize32=0);
int ubcDisconnect(int* sharedBuffer=0);
#endif 


#ifndef __NM__
int* ubcMalloc32(int sharedSize32);
void ubcFree(int* );
#else 
	//extern void*    ubcSharedBuffer;
	//extern unsigned ubcSharedSize;
	#include <string.h>
	inline int* ubcMalloc32(int sharedSize32) {return (int*)malloc(sharedSize32);}
	inline void ubcFree(int* shared) { free(shared);}
#endif


#endif  // _UBC_TARGET_H