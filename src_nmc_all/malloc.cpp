 #include "malloc.h"
 
void *malloc0(unsigned int size);
void *malloc1(unsigned int size);
void *malloc2(unsigned int size);
void *malloc3(unsigned int size);

 extern "C"{
	typedef void* (Malloc32Func)(unsigned int);	
	Malloc32Func* pMallocFunc=malloc;	
	void halSetActiveHeap(int heapNo){
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
	
	int* halMalloc32(int sharedSize32) {
		return (int*)pMallocFunc(sharedSize32);
	}	 
 };
