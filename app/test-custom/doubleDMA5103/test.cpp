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
		printf("CallBack\n");
		return 777;
	}
};
int main(){ 
	
	printf("******************* \n");
	
	
	int size32 = 512;
	nmppsMallocSetRoute16(0xF01);
	nm32s* src = nmppsMalloc_32s(size32 + 20);
	nm32s* dst = nmppsMalloc_32s(size32 + 20);
    nm32s* src1 = src + 100;
    nm32s* dst1 =dst + 100;

if (src==0 || dst==0)	return -1;
	
	printf("src  =%x \n",src);
	printf("dst  =%x \n",dst);
	
	printf("dmac0=%x \n",dmac0);
	printf("dmac1=%x \n",dmac1);
	
	for(int i=0;i<size32; i++){
		src[i]=i;
        src1[i]=i+0x1000;
		dst[i]=0;
        dst1[i]=0;
	}
	dmaopen5103();
	int ret;
	halSetCallbackDMA(user_callback);
	
	
	ret=halInitDoubleDMA((void*)src,(void*)src1,(void*)dst,(void*)dst1, 20,10);
	printf("exitCode of halInitDoubleDMA=%d \n",ret);
	
	if (ret==-1)
		return ret;
	ret=halStatusDMA();
	printf("status dma=%d \n",ret);
	//return ret;
	
	while (ret!=0){
		ret=halStatusDMA();
		//ret=dmac0();
		printf("status dma=%d \n",ret);
		halSleep(1000);
	}
	printf("%d \n",index);
	halSleep(1000);
	for(int i=0;i<20; i++)
		printf("%x %x \n ", src[i], dst[i]);
	for(int i=0;i<10; i++)
		printf("%x %x \n ", src1[i], dst1[i]);
	return index;

	return 777;		
}
