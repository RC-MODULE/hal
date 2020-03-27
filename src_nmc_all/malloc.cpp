#include "malloc.h"
#include "section-hal.h" 

#ifdef __GNUC__
extern "C"{
 	void *malloc0(unsigned int size);
	void *malloc1(unsigned int size);
	void *malloc2(unsigned int size);
	void *malloc3(unsigned int size);

	typedef void* (Malloc32Func)(unsigned int);	

	INSECTION(".data_hal")	Malloc32Func* pMallocFunc=malloc;	

	INSECTION(".text_hal")	void halSetActiveHeap(int heapNo){
		switch(heapNo){
			case(0):
				pMallocFunc=malloc0;
				break;
			case(1):
				pMallocFunc=malloc1;
				break;
			case(2):
				pMallocFunc=malloc2;
				break;
			case(3):
				pMallocFunc=malloc3;
				break;
		} 
	}
	
	INSECTION(".text_hal")	int* halMalloc32(int sharedSize32) {
		return (int*)pMallocFunc(sharedSize32);
	}	 
	
	
	

};
#endif
