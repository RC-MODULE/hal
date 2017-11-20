 #include "malloc.h"
 
 
 extern "C"{
	typedef void* (Malloc32Func)(unsigned int);	
	Malloc32Func* pMallocFunc;	
	void setActiveHeap(int heapNo){
		switch(heapNo){
			case(0):
				pMallocFunc=malloc;
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
	
	int* ubcMalloc32(int sharedSize32) {
		return (int*)pMallocFunc(sharedSize32);
	}	 
 };
