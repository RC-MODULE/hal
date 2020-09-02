#ifndef HAL_H_INCLUDED
#define HAL_H_INCLUDED
#include "led.h"
#include "dma.h"
#include "sleep.h"
#include "nmtype.h"
#include "stdlib.h"




#ifdef __NM__
//	typedef void nm8s ;
//	typedef void nm16s;
#define DISABLE_PRINTF() extern "C" {	int printf ( const char * format, ... ){	return 0;	}};
#else 
//	
//	typedef char nm8s;
//	typedef short nm16s;
#define DISABLE_PRINTF() 
#endif
	
//#endif	



#ifdef __NM__
	inline int* halMalloc32(int sharedSize32) {return (int*)malloc(sharedSize32); }
	inline void halFree(void* shared) { free(shared);}
#else 
	extern "C"{
	int* halMalloc32(int sharedSize32) ;//{return (int*)malloc(sharedSize32 * 4); }
	void halFree(void* );
	};
#endif
	
#ifdef __cplusplus
	extern "C" {
#endif

	void halEnterCriticalSectionCore(int coreID);
	void halExitCriticalSectionCore(int coreID);
	void halEnterCriticalSection();
	void halExitCriticalSection();
	
	void*  halCopyRISC  (const void* src,  void* dst,  unsigned size32);
	void*  halCopyDMA   (const void* src,  void* dst,  unsigned size32);
	void*  halCopyInt   (const void* src,  void* dst,  unsigned size32);
	void*  halCopyFlt   (const void* src,  void* dst,  unsigned size32);
	
	// отборажение  адреса в глобальное адресное пространство
	//void* halMapAddr(const void* srcAddr);	
	void* halMapAddrTo(const void* ownAddress, int toProccessor);
	void* halMapAddrFrom(const void* extAddress, int fromProcessor);

	void halSetActiveHeap(int heapNo);

	struct SyncBuf {
		int 	stateDMA;	// -1  free	
							// 0 used by core 0
							// 1 used by core 1
		//3 varibels below use for peterson algorithm in critical section
		int 	turn; 
		int 	flag0;
		int 	flag1;
		int		readCounter[2];
		int		writeCounter[2];
		int     sync0;
		int     sync1;
		int 	handshake[5];
		int 	ringBufferDMA;
		void*	syncAddr0;
		void*	syncAddr1;
	} ;

#ifdef __cplusplus
		};
#endif

	/**
    *
    *     \defgroup hal HAL функции 
    */
	
#endif 	

int			halGet_8s (const nm8s*  pVec, int nIndex);
int			halGet_16s(const nm16s* pVec, int nIndex);

