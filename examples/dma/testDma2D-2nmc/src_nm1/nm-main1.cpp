
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

int index = 777;

extern "C" int haltest();

extern "C"{
	int user_callback()
	{
		//halLedOn(7);
		halLedOn(6);

		return 777;
	}
};





int *src[4];
int *dst[4];
#define MAX_WIDTH 128
#define MAX_HEIGHT 32
#define MAX_SIZE MAX_WIDTH*MAX_HEIGHT

int main()
{


	halSetProcessorNo(1);							// Set processor number 
	halHandshake(0);
	halLedOn(4);
	int no=halGetProcessorNo();

	halDmaInitC();

	
	src[0]=(int*)malloc (MAX_SIZE+32);
	src[1]=(int*)malloc0(MAX_SIZE+32);
	src[2]=(int*)malloc1(MAX_SIZE+32);
	src[3]=(int*)malloc2(MAX_SIZE+32);
								 
	dst[0]=(int*)malloc (MAX_SIZE+32);
	dst[1]=(int*)malloc0(MAX_SIZE+32);
	dst[2]=(int*)malloc1(MAX_SIZE+32);
	dst[3]=(int*)malloc2(MAX_SIZE+32);
	
	if (
	src[0]==0 ||
	src[1]==0 ||
	src[2]==0 ||
	src[3]==0 ||
			
	dst[0]==0 ||
	dst[1]==0 ||
	dst[2]==0 ||
	dst[3]==0 
	){
		halLedSOS(4);
		//printf("memory allocation error\n");
		return -1;
	}
	halLedOn(5);
	
	
	src[0]=(int*)(((int)src[0]+32)&0xFFFFFFF0);
	src[1]=(int*)(((int)src[1]+32)&0xFFFFFFF0);
	src[2]=(int*)(((int)src[2]+32)&0xFFFFFFF0);
	src[3]=(int*)(((int)src[3]+32)&0xFFFFFFF0);
							  
	dst[0]=(int*)(((int)dst[0]+32)&0xFFFFFFF0);
	dst[1]=(int*)(((int)dst[1]+32)&0xFFFFFFF0);
	dst[2]=(int*)(((int)dst[2]+32)&0xFFFFFFF0);
	dst[3]=(int*)(((int)dst[3]+32)&0xFFFFFFF0);
			
	for(int i=0;i<4; i++){
		//printf("src[%d]:%x  dst[%d]:%x\n",i,src[i],i,dst[i]);
		for(int j=0; j<MAX_SIZE; j++)
			src[i][j]=j;
	}
	
	
	//if (0)
	for(int i=1; i<1000; i++){
		halLed4Hi(i);
		for(int j=1; j<1000; j++){
			int srcIdx	 = nmppcRandMinMax(0,3);
			int dstIdx	 = nmppcRandMinMax(0,3);
			int srcDisp  = nmppcRandMinMax(0,16) &(~1);
			int dstDisp  = nmppcRandMinMax(0,16) &(~1);
			int width    = nmppcRandMinMax(2,MAX_WIDTH) &(~1);
			int height   = nmppcRandMinMax(1,MAX_HEIGHT-1);
			int srcStride= nmppcRandMinMax(width,MAX_WIDTH)&(~1);
			int dstStride= nmppcRandMinMax(width,MAX_WIDTH)&(~1);
			int* in 	 = src[srcIdx]+srcDisp;
			int* out	 = dst[dstIdx]+dstDisp;
			halDma2D_StartC(in,out,width*height,width,srcStride,dstStride);
			while (!halDma2D_IsCompleted());
			unsigned srcCrc=nmppmCrc_32s(in, height,width, srcStride);
			unsigned dstCrc=nmppmCrc_32s(out, height,width,dstStride);
			if (srcCrc!=dstCrc){
				//printf("src crc=%x\n",srcCrc);
				//printMatrixS_32x("src:", in,   height, width, srcStride);
				//printMatrixS_32x("dst:", out,  height, width, dstStride);
	
				//printf("dst crc=%x\n",dstCrc);
				//printf("Error");
				halLedSOS(6);
				halSync(0,1);
				return -1;
				
			}
			else {
				//printf(".");
			}
		}
	}
	
	halSync(0,0);

	return 111;
}
