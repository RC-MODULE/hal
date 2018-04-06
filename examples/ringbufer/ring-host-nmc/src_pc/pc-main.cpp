#include "hal_host.h"
#include "stdio.h"
#include "windows.h"
#include "ringbuffer_host.h"
#include "sleep.h"


int main(){


	if (halOpen("main.abs"))
		return -1;
	
	unsigned sync= halSync(0x8086);
	if (sync!=0x6406)
		return sync;

	unsigned nmcSrcRingBufferAddr= halSync(0);
	unsigned nmcDstRingBufferAddr= halSync(1);
	
	HalHostRingBuffer srcRingBuffer;
	HalHostRingBuffer dstRingBuffer;

	
	halHostRingBufferInit(&srcRingBuffer, nmcSrcRingBufferAddr);
	halHostRingBufferInit(&dstRingBuffer, nmcDstRingBufferAddr);

	int* src= (int*)malloc(1024*4);
	int* dst= (int*)malloc(1024*4);

	for(int i=0; i<256; i++)
		src[i]=i;

	int counter=0;
	while(1){
		halHostRingBufferPush(&srcRingBuffer,src,256);
		halHostRingBufferPop (&dstRingBuffer,dst,256);

		//for(int i=0; i<256; i++)
		//	printf("src=%d \t dst=%d\n",src[i],dst[i]);
		printf("%d\n",counter++);
		
	}
	halClose();								// close board, disconect from shared memory
	halSleep(3000);

	free(src);
	free(dst);
	return sync;

}