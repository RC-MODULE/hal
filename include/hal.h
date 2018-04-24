#ifndef HAL_H_INCLUDED
#define HAL_H_INCLUDED
#include "led.h"
#include "dma.h"
#include "sleep.h"

#ifdef __NM__
#define DISABLE_PRINTF() extern "C" {	int printf ( const char * format, ... ){	return 0;	}};
#else 
#define DISABLE_PRINTF() 
#endif

#ifdef __cplusplus
		extern "C" {
#endif


	void halSetActiveHeap(int heapNo);

	struct SyncBuf {
		int 	stateDMA;	// -1  free	
							// 0 used by core 0
							// 1 used by core 1
		int 	turn;
		int 	flag0;
		int 	flag1;
		int		readCounter[2];
		int		writeCounter[2];
		int		sync0;
		int		sync1;
		
	
	} ;

#ifdef __cplusplus
		};
#endif

	/**
    *
    *     \defgroup hal HAL функции 
    */
	
#endif 	