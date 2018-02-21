#include "led.h"
#include "dma.h"


#ifdef __NM__
#define DISABLE_PRINTF() extern "C" {	int printf ( const char * format, ... ){	return 0;	}};
#else 
#define DISABLE_PRINTF() 
#endif

#ifdef __cplusplus
		extern "C" {
#endif


	void halSetActiveHeap(int heapNo);

#ifdef __cplusplus
		};
#endif

