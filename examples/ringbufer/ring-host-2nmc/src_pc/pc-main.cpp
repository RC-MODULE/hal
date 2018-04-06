#include "hal_host.h"
#include "stdio.h"
#include "windows.h"
#include "ringbuffer_host.h"
#include "sleep.h"
#include <process.h> 


int* src0;
int* dst0;
int* src1;
int* dst1;

HalHostRingBuffer* srcRingBuffer0;
HalHostRingBuffer* dstRingBuffer0;
HalHostRingBuffer* srcRingBuffer1;
HalHostRingBuffer* dstRingBuffer1;


void threadPush0(void*){
	int count=16;
	while(1){
		for(int i=0; i<count; i++)
			src0[i]+=count;
		halHostRingBufferPush(srcRingBuffer0,src0,count);
	}
}

void threadPop0(void*){
	int counter=0;
	while(1){
		halHostRingBufferPop(dstRingBuffer0,dst0,256);
		for(int i=0; i<16; i++)
			printf("dst0=%X \t dst1=%X\n",dst0[i],dst1[i]);
		printf("%d---------------------------\n",counter++);
	}
}

void threadPush1(void*){
	int count=12;
	while(1){
		for(int i=0; i<count; i++)
			src1[i]+=count;
		halHostRingBufferPush(srcRingBuffer1,src1,count);
	}
}
void threadPop1(void*){
	int counter=0;
	while(1){
		halHostRingBufferPop(dstRingBuffer1,dst1,16);
		for(int i=0; i<16; i++)
			printf("dst0=%X \t dst1=%X\n",dst0[i],dst1[i]);
		printf("%d---------------------------\n",counter++);

	}
}


int main(){


	if (halOpen("main0.abs","main1.abs"))
		return -1;
	
	unsigned sync;
	sync = halSync(0x8086,0);
	if (sync!=0x6406)
		return sync;
	sync= halSync(0x8086,1);
	if (sync!=0x6406)
		return sync;

	unsigned nmc0_SrcRingBufferAddr= halSync(0,0);
	unsigned nmc0_DstRingBufferAddr= halSync(1,0);
	unsigned nmc1_SrcRingBufferAddr= halSync(2,1);
	unsigned nmc1_DstRingBufferAddr= halSync(3,1);
	
	srcRingBuffer0 = new HalHostRingBuffer;
	dstRingBuffer0 = new HalHostRingBuffer;
	srcRingBuffer1 = new HalHostRingBuffer;
	dstRingBuffer1 = new HalHostRingBuffer;

	
	halHostRingBufferInit(srcRingBuffer0, nmc0_SrcRingBufferAddr,0);
	halHostRingBufferInit(dstRingBuffer0, nmc0_DstRingBufferAddr,0);
	halHostRingBufferInit(srcRingBuffer1, nmc1_SrcRingBufferAddr,1);
	halHostRingBufferInit(dstRingBuffer1, nmc1_DstRingBufferAddr,1);

	
	src0= (int*)malloc(1024*4);
	dst0= (int*)malloc(1024*4);
	src1= (int*)malloc(1024*4);
	dst1= (int*)malloc(1024*4);

	for(int i=0; i<256; i++){
		src0[i]=i;
		src1[i]=i+0x1000;
	}

	int		counter=0;

	_beginthread( threadPush0, 0, NULL );  
	_beginthread( threadPop0, 0, NULL );  
	_beginthread( threadPush1, 0, NULL );  
	_beginthread( threadPop1, 0, NULL );  
	
	
	
	int prev=0;
	while(1){
		//halHostRingBufferPush(srcRingBuffer0,src0,256);
		//halHostRingBufferPop (dstRingBuffer0,dst0,256);

		//halHostRingBufferPush(srcRingBuffer1,src1,256);
		//halHostRingBufferPop (dstRingBuffer1,dst1,256);
		if (0) if (dst1[0]!=prev){
			prev=dst1[0];
			for(int i=0; i<16; i++)
				printf("dst0=%X \t dst1=%X\n",dst0[i],dst1[i]);
			printf("%d---------------------------\n",counter++);
			
		}
		

		
		
	}
	halClose();								// close board, disconect from shared memory
	halSleep(3000);

	free(src0);
	free(dst0);
	free(src1);
	free(dst1);
	return sync;

}