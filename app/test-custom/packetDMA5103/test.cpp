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
    nm32s* dst1 = dst + 100;
	nm32s* src2 = src + 200;
    nm32s* dst2 = dst + 200;
	
	void* psrc[3];
	psrc[0]=src;
	psrc[1]=src1;
	psrc[2]=src2;

	
	void* pdst[3];
	pdst[0]=dst;
	pdst[1]=dst1;
	pdst[2]=dst2;

	int psize[4]={10,4,8,0};
	
	
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
	int ret;
	halSetCallbackDMA(user_callback);
	
	
	ret=halInitPacketDMA(psrc,pdst,psize);
	printf("exitCode of halInitPacketDMA=%d \n",ret);
	
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
	
	for(int i=0;i<3; i++)
		for(int j=0;j<psize[i]; j++)
			printf("%x %x \n ", ((int*)psrc[i])[j], ((int*)pdst[i])[j]);
	return index;

	return 777;		
}
