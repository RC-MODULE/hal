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

	void halHandshake(int processor);

	int halSyncArray(
					 int value,        // Sync value
					 void *outAddress, // Sended array address (can be NULL)
					 size_t outLen,    // Sended array length (can be 0)
					 void **inAddress, // Received array address pointer (can be NULL)
					 size_t *inLen,   // Received array size pointer (can be NULL)
					 int  procNo);

	int halSync(int val,int processor=0);
	void* halSyncAddr(void* sendAddr,int processor=0);
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


};

//template<class T> inline T* halMallocT(T* type){
//	return (T*)halMalloc32(sizeof32(T));
//}
//#define HAL_MALLOC_T(t) (t*)halMalloc32(sizeof32(t))

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
#endif // __NM__


//#ifndef _MACHINE_MALLOC_H_GUARD
//#define _MACHINE_MALLOC_H_GUARD

//#ifndef __NM__
#define NMC_MAX_HEAP_NUM 32
#define NMC_HEAP_NOT_SET (-1)

#ifdef	__cplusplus
extern "C"
{
#endif

void nmc_malloc_set_heap (int h_num);
int  nmc_malloc_get_heap (void) ;

#ifdef	__cplusplus
}
#endif

//#endif // __NM__

//#endif  // _MACHINE_MALLOC_H




#endif  // _UBC_TARGET_H