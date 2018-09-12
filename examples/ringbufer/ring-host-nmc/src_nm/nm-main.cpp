//------------------------------------------------------------------------
//
//
//  Copyright (c) RC Module Inc.
//
//!
//! \file   main.cpp
//! \author S. Mushkaev
//! \brief  Example of universl-board-connect usage
//!
//! \endif
//!
//------------------------------------------------------------------------

#include "ringbuffer.h"
#include "hal_target.h"
#include "dma.h"	
#include "nmpp.h"
#include "stdio.h"
#include "string.h"


#define MEM_OFFSET 0x40000
#if defined(MC12101)
	#if defined(NMC0)
		#define IN_MIRROR(addr) ((int)addr<MEM_OFFSET?(void*)((int)addr+MEM_OFFSET):addr)
	#elif defined(NMC1)
		#define IN_MIRROR(addr) ((int)addr<MEM_OFFSET?(void*)((int)addr+MEM_OFFSET):addr)
	#endif
#else 
	#define IN_MIRROR(addr) addr
#endif 


extern "C"{
	int riscSingleCopy(void* src, void* dst, int size){
#ifdef __NM__
		memcpy(dst,src,size);
#else 
		memcpy(dst, src, size*4);
#endif

		return 0;
	}

int riscDoubleCopy(void* src0,void* src1, void* dst0, void* dst1, int size0,int size1){
#ifdef __NM__
		memcpy(dst0,src0,size0);
		memcpy(dst1,src1,size1);
#else 
		memcpy(dst0, src0, size0*4);
		memcpy(dst1, src1, size1*4);
#endif
		return 0;
	}
};

int main()
{
	unsigned  sync;
	halLedOn(0);
	sync=halHostSync(0x6406);	// Handshake 
	printf("Start\n");
	//halEnbExtInt();
	//halMaskIntContMdma_mc12101();
	//halInitDMA();

	int itemSize= 1;
	int count=1024;
	
	nmppsMallocSetRoute16(0xF1100);
	int* srcBuffer = (int*)nmppsMalloc_32s(itemSize*count);
	int* dstBuffer = (int*)nmppsMalloc_32s(itemSize*count);
	int* src    = (int*)nmppsMalloc_32s(1024);
	int* dst    = (int*)nmppsMalloc_32s(1024);

	HalRingBuffer* srcRing = (HalRingBuffer*)halMalloc32(sizeof(HalRingBuffer));
	HalRingBuffer* dstRing = (HalRingBuffer*)halMalloc32(sizeof(HalRingBuffer));
	
	if (srcBuffer==0 || dstBuffer==0 || src==0 || dst==0) 
		halLedSOS(0);
	
	if (int ret=halRingBufferInit(srcRing,IN_MIRROR(srcBuffer),itemSize,count, halInitSingleDMA, halInitDoubleDMA, halSetCallbackDMA)){
		halLed(ret);
		return -1;
	}
	if (int ret=halRingBufferInit(dstRing,IN_MIRROR(dstBuffer),itemSize,count, halInitSingleDMA, halInitDoubleDMA, halSetCallbackDMA)){
		halLed(ret);
		return -1;
	}
	
	sync=halHostSync((unsigned)srcRing);	// Gets array size, sends input buffer address
	sync=halHostSync((unsigned)dstRing);	// Gets increment (123), sends output buffer address
	
	while(1){
		halRingBufferPop(srcRing,IN_MIRROR(src),256);
		halLed(1);
		while(srcRing->pendingCount);
		halLed(2);
		for(int i=0; i<256;i++)
			dst[i]=~src[i];
		halRingBufferPush(dstRing,IN_MIRROR(dst),256);
		halLed(3);
		while(srcRing->pendingCount);
		halLed(4);
	}

	halFree(srcBuffer);					// dealocates sharedBuffer
	halFree(dstBuffer);					// dealocates sharedBuffer
	halFree(src);						// dealocates sharedBuffer
	halFree(dst);						// dealocates sharedBuffer
	halLed(0x55);
	return 0x600D+(halGetProcessorNo()<<28);
}

