
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

#include <stdio.h>
#include "hal_target.h"
#include "sleep.h"
#include "hal.h"
#include "cache.h"
//DISABLE_PRINTF();
#include "ringbuffert.h"
#include "dma.h"
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


#include <stdio.h>
#include <string.h>
#include "hal_target.h"
#include "sleep.h"
#include "hal.h"
#include "led.h"
#include "nmpp.h"

int index = 777;

extern "C" int haltest();

extern "C"{
	int user_callback()
	{
		//halLedOn(7);
		halLedOn(6);
		index =999;
		return 777;
	}
};

void* halMap2MDMA(const void* addr){
	//if (core==1){
		if ((unsigned)addr <0x80000)	// NMC1 INT
			return (void*)(0x80000+(unsigned)addr);
		return (void*)addr;
	//}
}

void*  halCopyDMA   (const void* src,  void* dst,  unsigned size32){
	//DmaCallback originCallback=halGetCallbackDMA();
	//halOpenDMA();
	//halLedOff(5);
	//halLedOff(6);
	//halLedOff(7);
	
	int ret;
	//halSetCallbackDMA(dummy);
	//ret = halCheckParamsSingleDMA((void*)src, dst, size32);
	//if (ret){
	//	printf("ERORR: wrong parametrs in DMA initialisation exit code is %d\n",ret);
	//	//return ret;
	//}
	//printf("ok \n");
	//halSetDummyCallbackDMA();
	//printf("%x => %x\n",halMap2MDMA(src), halMap2MDMA(dst));
	//halLedOn(5);
	//halSetCallbackDMA(user_callback);
	//halEnterCriticalSection1();
	halInitSingleDMA((void*)src, dst, size32);
	//halInitSingleDMA((void*)halMap2MDMA(src), halMap2MDMA(dst), size32);
	//halInitStatusSingleDMA((void*)halMap2MDMA(src), halMap2MDMA(dst), size32);
	//halLedOn(6);
	//printf("ok \n");
	do {
		ret=halStatusDMA();
		//printf("%d",ret);
		//halSleep(10);
	} while (ret!=2);
	//halExitCriticalSection1();
	
	//halLedOn(7);
	//halSetCallbackDMA(originCallback);
	
	return dst;
}



int *src[4];
int *dst[4];
#define MAX_SIZE 128
//128*4
extern "C" int TEST(int);
int main()
{
	
	//TEST(3);
	//halLedSOS(7,2);
	//return 1;
	int g;
	//int g;


	//halSetProcessorNo(1);				// Set processor number 
	//halLed(3);
	int no=halGetProcessorNo();
	halHandshake(0);
	no = *((int*)(0x40000000));
	//printf("%x\n",no>>24);
	//return no>>24;
	//halLedOn(4);
	g=halOpenDMA();
	halSync(1,0);
	//return g;
	
	src[0]=(int*)malloc (MAX_SIZE+32);
	src[1]=(int*)malloc0(MAX_SIZE+32);
	src[2]=(int*)malloc1(MAX_SIZE+32);
	src[3]=(int*)malloc2(MAX_SIZE+32);
								 
	dst[0]=(int*)malloc (MAX_SIZE+32);
	dst[1]=(int*)malloc0(MAX_SIZE+32);
	dst[2]=(int*)malloc1(MAX_SIZE+32);
	dst[3]=(int*)malloc2(MAX_SIZE+32);
	
	src[0]=(int*)(((int)src[0]+32)&0xFFFFFFF0);
	src[1]=(int*)(((int)src[1]+32)&0xFFFFFFF0);
	src[2]=(int*)(((int)src[2]+32)&0xFFFFFFF0);
	src[3]=(int*)(((int)src[3]+32)&0xFFFFFFF0);
							  
	dst[0]=(int*)(((int)dst[0]+32)&0xFFFFFFF0);
	dst[1]=(int*)(((int)dst[1]+32)&0xFFFFFFF0);
	dst[2]=(int*)(((int)dst[2]+32)&0xFFFFFFF0);
	dst[3]=(int*)(((int)dst[3]+32)&0xFFFFFFF0);
	
			
	for(int i=0;i<4; i++){
		printf("src[%d]:%x  dst[%d]:%x\n",i,src[i],i,dst[i]);
		for(int j=0; j<MAX_SIZE; j++)
			src[i][j]=j;
	}
	
	//halSetCallbackDMA(user_callback);
	if (0) for(int i=0; i<1600;i++){
		
		//halSync(0,0);
		halCopyDMA(src[0],dst[0],16);
		//halSync(0,0);
		unsigned srcCrc=nmppsCrc_32s(src[0],16);
		unsigned dstCrc=nmppsCrc_32s(dst[0],16);
		if (srcCrc!=dstCrc){
			//printf("Error");
			halLedSOS(7);
			
		}
	}
	//halCopyDMA(src[0],dst[0],16);
	//halCopyDMA(src[0],dst[0],16);
	
	if (1)
	for(int k=1; k<20000000; k++){
			halLed4Hi(k);
	for(int i=0;i<4 ;i++){
			//halLed4Hi(i*j);
		for(int j=0; j<4; j++){
			for(int srcDisp=0; srcDisp<16; srcDisp+=2){
				for(int dstDisp=0; dstDisp<16; dstDisp+=2){
					for(int size=16; size<MAX_SIZE; size+=2){
						//halEnterCriticalSection();
						//halLedOffSC(0);
						//halLedOffSC(1);
						halCopyDMA(src[i]+srcDisp,dst[i]+dstDisp,size);
						//halExitCriticalSection();
						unsigned srcCrc=nmppsCrc_32s(src[i]+srcDisp,size);
						unsigned dstCrc=nmppsCrc_32s(dst[i]+dstDisp,size);
						if (srcCrc!=dstCrc){
							//printf("Error");
							halLedSOS(7);
							
						}
					}
				}
			}
		}
	}
	}
	
	halSync(0,0);
	//halSleep(1);
	return 111;
}
