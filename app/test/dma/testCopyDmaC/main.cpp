//------------------------------------------------------------------------
//
//
//  Copyright (c) RC Module Inc.
//
//!
//! \file   main.cpp
//! \author S. Mushkaev
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
#include "malloc.h"

#ifndef __GNUC__


void nmc_malloc_set_heap(int heap){

}

#endif


void* malloc0(int size){
	nmc_malloc_set_heap(0);
	return malloc(size*4);
}
void* malloc1(int size) {
	nmc_malloc_set_heap(1);
	return malloc(size*4);
}
void* malloc2(int size) {
	nmc_malloc_set_heap(2);
	return malloc(size*4);
}
void* malloc3(int size) {
	nmc_malloc_set_heap(3);
	return malloc(size*4);
}



#include "qdma.h"
#include "halcrc.h"

extern "C" int haltest();

#define MAX_SIZE 128
int *src[4];
int *dst[4];


void*  copyDma(const void* src,  void* dst,  unsigned size32){
	halDmaStart((void*)src, dst, size32);
	while (!halDmaIsCompleted()){
		//halSleep(10);
	}
	return dst;
}





int testCopyDma(int count, DmaStart copyFunc){
	for(int k=0; k<count; k++){
		halLed4Lo(k);
		for(int i=0;i<4 ;i++){												// src bank index
			for(int j=0; j<4; j++){											// dst bank index
				for(int srcDisp=0; srcDisp<16; srcDisp+=2){					// srcDisp
					for(int dstDisp=0; dstDisp<16; dstDisp+=2){				// dstDisp
						for(int size=2; size<MAX_SIZE; size+=2){			// size
							copyFunc(src[i]+srcDisp,dst[j]+dstDisp,size);
							unsigned srcCrc=halCrc_32s(src[i]+srcDisp,size);
							unsigned dstCrc=halCrc_32s(dst[j]+dstDisp,size);
							if (srcCrc!=dstCrc){
								printf("Error");
								halLed(0xFF);
								halLedSOS(6);
								return -1;
							}
						}
					}
				}
			}
		}
	}
	return 	777;
}


int testCopyDmaA(int count, DmaStart copyFunc){
	for(int k=0; k<count; k++){
		halLed4Lo(k);
		for(int i=0;i<4 ;i++){												// src bank index
			for(int j=0; j<4; j++){											// dst bank index
				for(int size=2; size<MAX_SIZE; size+=2){					// size
					copyFunc(src[i],dst[j],size);
					unsigned srcCrc=halCrc_32s(src[i],size);
					unsigned dstCrc=halCrc_32s(dst[j],size);
					if (srcCrc!=dstCrc){
						printf("Error");
						halLed(0xFF);
						halLedSOS(6);
						return -1;
					}
				}
			}
		}
	}
	return 	777;
}


int testCopyDmaM(int count, DmaStart copyFunc){
	for(int k=0; k<count; k++){
		halLed4Lo(k);
		for(int i=0;i<4 ;i++){												// src bank index
			for(int j=0; j<4; j++){											// dst bank index
				for(int size=2; size<MAX_SIZE; size+=2){					// size
					int* s=(int*)halMapAddr(src[i]);
					int* d=(int*)halMapAddr(dst[j]);
					copyFunc(s,d,size);
					unsigned srcCrc=halCrc_32s(s,size);
					unsigned dstCrc=halCrc_32s(d,size);
					if (srcCrc!=dstCrc){
						printf("Error");
						halLed(0xFF);
						halLedSOS(6);
						return -1;
					}
				}
			}
		}
	}
	return 	777;
}

int testCallback(int count, DmaStart copyFunc){
	for(int k=0; k<count; k++){
		for(int i=0;i<4 ;i++){												// src bank index
			for(int j=0; j<4; j++){											// dst bank index
				for(int size=2; size<MAX_SIZE; size+=2){					// size
					int* s=(int*)halMapAddr(src[i]);
					int* d=(int*)halMapAddr(dst[j]);
					copyFunc(s,d,size);
					//halSleep(10);
					unsigned srcCrc=halCrc_32s(s,size);
					unsigned dstCrc=halCrc_32s(d,size);
					if (srcCrc!=dstCrc){
						printf("Error");
						halLed(0xFF);
						halLedSOS(6);
						return -1;
					}
				}
			}
		}
	}
	return 	777;
}

extern "C" {
	int dmaCallback()
	{
		static int calls=0;
		halLed(calls++);
		return 777;
	}
};

int main()
{
	
	
	src[0]=(int*)malloc0 (MAX_SIZE+32);
	src[1]=(int*)malloc0(MAX_SIZE+32);
	src[2]=(int*)malloc1(MAX_SIZE+32);
	src[3]=(int*)malloc2(MAX_SIZE+32);
								 
	dst[0]=(int*)malloc0 (MAX_SIZE+32);
	dst[1]=(int*)malloc0(MAX_SIZE+32);
	dst[2]=(int*)malloc1(MAX_SIZE+32);
	dst[3]=(int*)malloc2(MAX_SIZE+32);
	
	for(int i=0; i<3 ;i++)
		if (src[i]==0 || dst[i]==0){
			halLed(0xFF);
			return 0xBEDA;
		}
	
	// align buffers
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
	
	
	halDmaInit();
	testCopyDma(3, 	  halDmaStart);
	testCopyDmaA(100, halDmaStartA);
	testCopyDmaM(100, halDmaStartM);
	
	int err = 0;
	halDmaSetCallback(dmaCallback); 	printf("--\n");
	err |=testCallback(2,halDmaStart);  		printf("--\n");
	err |= testCallback(2,halDmaStartA);		printf("--\n");
	err |= testCallback(2,halDmaStartM);
	
	halDmaInitC();
	err |= testCopyDma (3,   halDmaStartC);
	err |= testCopyDmaA(100, halDmaStartCA);
	err |= testCopyDmaM(100, halDmaStartCM);
	
	return err;
}
