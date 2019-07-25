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
#include "ringbuffert.h"
#include "nmpp.h"
#include "qdma.h"

extern "C" int haltest();


void* halMap2MDMA(const void* addr){
	//if (core==0){
		if ((unsigned)addr <0x40000)	// NMC0 INT
			return (void*)(0x40000+(unsigned)addr);
		else return (void*)addr;
	//}
}

void*  halCopyDMA   (const void* src,  void* dst,  unsigned size32){
	//DmaCallback originCallback=halGetCallbackDMA();
	//halOpenDMA();
	//halLedOff(1);
	//halLedOff(2);
	//halLedOff(3);
	
	//halSetCallbackDMA(dummy);
	//int ret = halCheckParamsSingleDMA((void*)src, dst, size32);
	//if (ret){
	//	printf("ERORR: wrong parametrs in DMA initialisation exit code is %d\n",ret);
		//return ret;
	//}
	//printf("ok \n");
	//halSetDummyCallbackDMA();
	//printf("%x => %x\n",halMap2MDMA(src), halMap2MDMA(dst));
	//halLedOn(1);
	//halInitSingleDMA((void*)halMap2MDMA(src), halMap2MDMA(dst), size32);
	int *g=(int*)dst;
	
	//printf(" %x %x %x\n",g[0],g[1],g[2]);
	
	
	//halEnterCriticalSection0();
	//printf("dma start  \n");
	halInitSingleDMA((void*)src, dst, size32);
	//halInitSingleDMA((void*)halMap2MDMA(src), halMap2MDMA(dst), size32);
	//halLedOn(2);
	//printf("dma init fininsh  \n");
	
	int ret;
	do {
		ret=halStatusDMA();
		
		//printf(" %x %x %x -%d\n",g[0],g[1],g[2],ret);
		//halSleep(10);
	} while (ret!=2);
	//printf("status ok \n");
	//printf(" %x %x %x\n",g[0],g[1],g[2]);
	//halExitCriticalSection0();
	//halLedOn(3);
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
	//int g=
	//TEST(1);
	//halLedSOS(3,3);
	//return 0;
	int g;
	//halLedSOS(0);

	//halLed(0);
	halSetProcessorNo(0);							// Set processor number 
	halHandshake(1);
	int no=halGetProcessorNo();
	no = *((int*)(0x40000000));
	//return no>>24;
	//printf("%x\n",no>>24);
	//halLedOn(0);// reset all
	g=halOpenDMA();
	halSync(0,1);
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
	
	
	if (0) for(int i=0; i<1600;i++){
		halCopyDMA(src[0],dst[0],16);
		//halSync(0,1);
		//halSync(0,1);
		
		//halSync(0,0);
		unsigned srcCrc=nmppsCrc_32s(src[0],16);
		unsigned dstCrc=nmppsCrc_32s(dst[0],16);
		if (srcCrc!=dstCrc){
			//printf("Error");
			halLedSOS(6);
		}
	}

	
	if (1)
	for(int k=1; k<20000000; k++){
				halLed4Lo(k);
		for(int i=0;i<4 ;i++){
			for(int j=0; j<4; j++){
				//halLed4Lo(j*k);
				for(int srcDisp=0; srcDisp<16; srcDisp+=2){
					//halLedOn(6);
					for(int dstDisp=0; dstDisp<16; dstDisp+=2){
						for(int size=16; size<MAX_SIZE; size+=2){
							//halLedOn(0);
							//halLedOffSC(1);
							halCopyDMA(src[i]+srcDisp,dst[i]+dstDisp,size);
							unsigned srcCrc=nmppsCrc_32s(src[i]+srcDisp,size);
							unsigned dstCrc=nmppsCrc_32s(dst[i]+dstDisp,size);
							if (srcCrc!=dstCrc){
								//printf("Error");
								halLedSOS(6);
							}
							
						}
					}
				}
			}
		}
	}
	
	halSync(0,1);
	//halSleep(100);
	return 111;
}
