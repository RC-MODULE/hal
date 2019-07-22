#include "time.h"
#include "nmpp.h"
#include "dma.h"
#include "hal.h"
#include "stdio.h"
#include <string.h>
#include "dma5103.h"
#define Heap_size 8000
#define DDR_add_beging 0x20000000;

int index = 777;

extern "C"{
	int user_callback()
	{
		index =999;
		return 777;
	}
};
//#define printf(a,b)
int main(){ 
	
	int size32 = 100;
	
	nmppsMallocSetRoute16(0xF01);
	
	
	int* dst = nmppsMalloc_32s(size32 + 20);
	
	int* src = nmppsMalloc_32s(size32 + 20);
	
	if (src==0 || dst==0)	return -1;
	
	
	printf("src = %x \n",src);
	printf("dst = %x \n",dst);
	
	for(int i=0;i<size32; i++){
		src[i]=i;
		dst[i]=0;
	}
	
	halOpenDMA();
	halSetCallbackDMA(user_callback);
	int ret = halCheckParamsSingleDMA(src, dst, size32);
	if (ret){
		printf("ERORR: wrong parametrs in DMA initialisation exit code is %d\n",ret);
		return ret;
	}
	
	halInitSingleDMA(src, dst+0x80000, size32);
	
	do {
		ret=halStatusDMA();
		printf("status dma=%d \n",ret);
		//halSleep(1);
	} while (ret);
	
	for(int i=0; i<16; i++)
		printf("%x %x \n ", src[i], dst[i]);

	return index;
}
