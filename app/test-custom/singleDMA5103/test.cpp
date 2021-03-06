#include "time.h"
#include "nmpp.h"
#include "dma.h"
#include "hal.h"
#include "stdio.h"
#include <string.h>
#include "dma5103.h"
#define Heap_size 8000
#define DDR_add_beging 0x20000000;
extern "C"{
	
	int index = 1;

	int user_callback()
	{
		index =999;
//		printf("CallBack\n");
		return 777;
	}
};
int *src, *dst, ret;
int main(){ 
	
	printf("******************* \n");
	
	
	int size32 = 512;
	nmppsMallocSetRoute16(0xF01);
	dst = nmppsMalloc_32s(size32 + 20);
	src = nmppsMalloc_32s(size32 + 20);
	if (src==0 || dst==0)	return -1;
	
	printf("src  =%x \n",src);
	printf("dst  =%x \n",dst);
	
	printf("dmac0=%x \n",dmac0);
	printf("dmac1=%x \n",dmac1);
	
	for(int i=0;i<size32; i++){
		src[i]=i;
		dst[i]=0;
	}
	dmaopen5103();
	
	halSetCallbackDMA(user_callback);
	
	printf("set  \n");
	
	ret=halInitSingleDMA((void*)src,(void*)dst, 20);
	printf("exitCode of halInitSingleDMA=%d \n",ret);
	
	ret=halStatusDMA();
	printf("status dma=%d \n",ret);
	
	while (ret!=0){
		ret=halStatusDMA();
		printf("status dma=%d \n",ret);
		halSleep(1000);
	}
	printf("%d \n",index);
	halSleep(1000);
	for(int i=0;i<20; i++)
		printf("%x %x \n ", src[i], dst[i]);
	return index;

	return 777;		
}
